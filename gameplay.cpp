#include "gameplay.h"
#include "status.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

Gameplay::Gameplay(SDL_Renderer* renderer, int size, TTF_Font* font)
    : renderer(renderer), font(font), gridSize(size), gameOver(false), win(false), lives(3) {

    SDL_Surface* bgSurface = IMG_Load("assets/Bg.png");
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    SDL_Surface* heartAliveSurface = IMG_Load("assets/Alive.png");
    SDL_Surface* heartDeadSurface = IMG_Load("assets/Dead.png");
    heartAlive = SDL_CreateTextureFromSurface(renderer, heartAliveSurface);
    heartDead = SDL_CreateTextureFromSurface(renderer, heartDeadSurface);
    SDL_FreeSurface(heartAliveSurface);
    SDL_FreeSurface(heartDeadSurface);

    int maxWidth = 600;
    int maxHeight = 450;
    cellSize = std::min(maxWidth / gridSize, maxHeight / gridSize);
    offsetX = (800 - gridSize * cellSize) / 2;
    offsetY = 140;

    fontClue = TTF_OpenFont("assets/Roboto.ttf", 16);

    srand(static_cast<unsigned int>(time(nullptr)));

    solution.resize(gridSize, std::vector<int>(gridSize, 0));
    playerMatrix.resize(gridSize, std::vector<int>(gridSize, 0));

    for (int i = 0; i < gridSize; ++i)
        for (int j = 0; j < gridSize; ++j)
            solution[i][j] = rand() % 2;

    rowClues.resize(gridSize);
    colClues.resize(gridSize);

    for (int i = 0; i < gridSize; ++i) {
        int count = 0;
        for (int j = 0; j < gridSize; ++j) {
            if (solution[i][j] == 1)
                count++;
            else if (count > 0) {
                rowClues[i].push_back(count);
                count = 0;
            }
        }
        if (count > 0)
            rowClues[i].push_back(count);
        if (rowClues[i].empty())
            rowClues[i].push_back(0);
    }

    for (int j = 0; j < gridSize; ++j) {
        int count = 0;
        for (int i = 0; i < gridSize; ++i) {
            if (solution[i][j] == 1)
                count++;
            else if (count > 0) {
                colClues[j].push_back(count);
                count = 0;
            }
        }
        if (count > 0)
            colClues[j].push_back(count);
        if (colClues[j].empty())
            colClues[j].push_back(0);
    }
}

Gameplay::~Gameplay() {
    if (fontClue) {
        TTF_CloseFont(fontClue);
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(heartAlive);
    SDL_DestroyTexture(heartDead);
}

void Gameplay::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    for (int i = 0; i < 3; ++i) {
        SDL_Rect heartRect = {630 + i * 50, 30, 55, 55};
        if (i < lives) {
            SDL_RenderCopy(renderer, heartAlive, nullptr, &heartRect);
        } else {
            SDL_RenderCopy(renderer, heartDead, nullptr, &heartRect);
        }
    }
    renderGrid();
    renderClues();
    renderStatus(renderer, win, isLose(), lives);
}

void Gameplay::handleEvents(SDL_Event& e) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (e.type == SDL_MOUSEBUTTONDOWN && !gameOver) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        handleCellClick(e, x, y);
    }
    if (win || isLose()) return;
}

void Gameplay::renderGrid() {
    SDL_SetRenderDrawColor(renderer, 77, 108, 168, 255);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            SDL_Rect cell = { offsetX + j * cellSize, offsetY + i * cellSize, cellSize, cellSize };
            SDL_RenderFillRect(renderer, &cell);
        }
    }

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            SDL_Rect cell = { offsetX + j * cellSize + 1, offsetY + i * cellSize + 1, cellSize - 2, cellSize - 2 };
            if (playerMatrix[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
                SDL_RenderFillRect(renderer, &cell);
            } else if (playerMatrix[i][j] == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, cell.x, cell.y, cell.x + cell.w, cell.y + cell.h);
                SDL_RenderDrawLine(renderer, cell.x + cell.w, cell.y, cell.x, cell.y + cell.h);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (int i = 0; i <= gridSize; ++i) {
        SDL_RenderDrawLine(renderer, offsetX, offsetY + i * cellSize, offsetX + gridSize * cellSize, offsetY + i * cellSize);
        SDL_RenderDrawLine(renderer, offsetX + i * cellSize, offsetY, offsetX + i * cellSize, offsetY + gridSize * cellSize);
    }

    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    for (int i = 0; i <= gridSize; ++i) {
        if (i % 5 == 0) {
            SDL_RenderDrawLine(renderer, offsetX, offsetY + i * cellSize, offsetX + gridSize * cellSize, offsetY + i * cellSize);
            SDL_RenderDrawLine(renderer, offsetX + i * cellSize, offsetY, offsetX + i * cellSize, offsetY + gridSize * cellSize);
        }
    }
}

TTF_Font* Gameplay::getFittingFont(const std::string& text, int maxWidth, int initialSize) {
    TTF_Font* tryFont = nullptr;
    int fontSize = initialSize;

    while (fontSize > 6) {
        tryFont = TTF_OpenFont("assets/Roboto.ttf", fontSize);
        if (!tryFont) break;

        int w = 0, h = 0;
        TTF_SizeText(tryFont, text.c_str(), &w, &h);
        if (w <= maxWidth) return tryFont;

        TTF_CloseFont(tryFont);
        fontSize--;
    }

    return nullptr;
}

void Gameplay::renderClues() {
    SDL_Color textColor = {75, 0, 130, 255 };
    const int clueSpacing = 16;

    //Row clues
    for (int i = 0; i < gridSize; ++i) {
        int totalWidth = 0;
        std::vector<SDL_Texture*> textures;
        std::vector<SDL_Rect> rects;

        for (int clue : rowClues[i]) {
            std::string clueStr = std::to_string(clue);
            TTF_Font* fitFont = getFittingFont(clueStr, cellSize, 16);
            SDL_Surface* surface = TTF_RenderText_Blended(fitFont ? fitFont : fontClue, clueStr.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect rect = { 0, 0, surface->w, surface->h };
            textures.push_back(texture);
            rects.push_back(rect);

            totalWidth += surface->w + 5;

            SDL_FreeSurface(surface);
            if (fitFont) TTF_CloseFont(fitFont);
        }

        int xStart = offsetX - totalWidth - 5;
        for (size_t k = 0; k < textures.size(); ++k) {
            SDL_Rect dstRect = {
                xStart,
                offsetY + i * cellSize + (cellSize - rects[k].h) / 2,
                rects[k].w,
                rects[k].h
            };
            SDL_RenderCopy(renderer, textures[k], nullptr, &dstRect);
            xStart += rects[k].w + 5;
            SDL_DestroyTexture(textures[k]);
        }
    }

    //Column clues
    for (int j = 0; j < gridSize; ++j) {
        int yStart = offsetY - (int)colClues[j].size() * clueSpacing - 5;

        for (size_t k = 0; k < colClues[j].size(); ++k) {
            std::string clueStr = std::to_string(colClues[j][k]);
            TTF_Font* fitFont = getFittingFont(clueStr, cellSize, 16);
            SDL_Surface* surface = TTF_RenderText_Blended(fitFont ? fitFont : fontClue, clueStr.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect dstRect = {
                offsetX + j * cellSize + (cellSize - surface->w) / 2,
                (int)(yStart + k * clueSpacing),
                surface->w,
                surface->h
            };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            if (fitFont) TTF_CloseFont(fitFont);
        }
    }
}

void Gameplay::checkWin() {
    for (int i = 0; i < gridSize; ++i)
        for (int j = 0; j < gridSize; ++j)
            if ((playerMatrix[i][j] == 1 && solution[i][j] != 1) ||
                (solution[i][j] == 1 && playerMatrix[i][j] != 1)) {
                return;
            }

    win = true;
    gameOver = true;
}

bool Gameplay::isLose() const {
    return lives <= 0;
}

void Gameplay::handleCellClick(SDL_Event e, int mouseX, int mouseY) {
    int row = (mouseY - offsetY) / cellSize;
    int col = (mouseX - offsetX) / cellSize;

    if (row >= 0 && row < gridSize && col >= 0 && col < gridSize) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            playerMatrix[row][col] = 1;
            if (solution[row][col] != 1) {
                lives--;
            }
        } else if (e.button.button == SDL_BUTTON_RIGHT) {
            playerMatrix[row][col] = 2;
        }
        checkWin();
    }
}
