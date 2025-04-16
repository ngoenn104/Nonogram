#include "gameplay.h"
#include "status.h"
#include "game.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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

    clickSound = Mix_LoadWAV("assets/click.wav");
    Mix_VolumeChunk(clickSound, 45);

    int maxWidth = 600;
    int maxHeight = 450;
    cellSize = std::min(maxWidth / gridSize, maxHeight / gridSize);
    offsetX = (800 - gridSize * cellSize) / 2;
    offsetY = 140;

    fontClue = TTF_OpenFont("assets/Roboto.ttf", 16);

    srand(static_cast<unsigned int>(time(nullptr)));
    Game game(gridSize);
    solution = game.getMatrix();
    rowClues = game.getRowClues();
    colClues = game.getColClues();
    playerMatrix.resize(gridSize, std::vector<int>(gridSize, 0));
}

Gameplay::~Gameplay() {
    if (fontClue) {
        TTF_CloseFont(fontClue);
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(heartAlive);
    SDL_DestroyTexture(heartDead);

    Mix_FreeChunk(clickSound);
}

void Gameplay::handleEvents(SDL_Event& e) {
    if ((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONUP) && !gameOver) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        int row = (y - offsetY) / cellSize;
        int col = (x - offsetX) / cellSize;

        if (row >= 0 && row < gridSize && col >= 0 && col < gridSize) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                mouseIsBusy = true;
                startRowWhenDrag = row;
                startColWhenDrag = col;
                mouseClicking = e.button.button;
                drawWithMouse(row, col, mouseClicking);
            }
            else if (e.type == SDL_MOUSEMOTION && mouseIsBusy) {
                drawWithMouse(row, col, mouseClicking);
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                mouseIsBusy = false;
                startRowWhenDrag = startColWhenDrag = -1;
            }
        }
    }
    if (win || isLose()) return;
}

void Gameplay::drawWithMouse(int row, int col, Uint8 button) {
    if (row != startRowWhenDrag && col != startColWhenDrag) return;

    int r1 = std::min(startRowWhenDrag, row);
    int r2 = std::max(startRowWhenDrag, row);
    int c1 = std::min(startColWhenDrag, col);
    int c2 = std::max(startColWhenDrag, col);

    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            if (button == SDL_BUTTON_LEFT) {
                if (playerMatrix[r][c] != 1) {
                    playerMatrix[r][c] = 1;
                    if (solution[r][c] != 1) lives--;
                    Mix_PlayChannel(-1, clickSound, 0);
                }
            } else if (button == SDL_BUTTON_RIGHT) {
                if (playerMatrix[r][c] != 2) {
                    playerMatrix[r][c] = 2;
                    Mix_PlayChannel(-1, clickSound, 0);
                }
            }
        }
    }
    checkWin();
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

    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
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

void Gameplay::renderOneClue(const std::string& clueStr, int x, int y) {
    SDL_Color textColor = {75, 0, 130, 255};
    TTF_Font* font = getFittingFont(clueStr, cellSize, 16);
    SDL_Surface* surface = TTF_RenderText_Blended(font ? font : fontClue, clueStr.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    if (font) TTF_CloseFont(font);
}

void Gameplay::renderClues() {
    const int clueSpacing = 16;

    // Row clues
    for (int row = 0; row < gridSize; ++row) {
        int totalWidth = 0;
        std::vector<std::string> clues;
        std::vector<SDL_Point> sizes;

        for (int clue : rowClues[row]) {
            std::string clueStr = std::to_string(clue);
            TTF_Font* font = getFittingFont(clueStr, cellSize, 16);
            int w = 0, h = 0;
            TTF_SizeText(font ? font : fontClue, clueStr.c_str(), &w, &h);
            if (font) TTF_CloseFont(font);

            clues.push_back(clueStr);
            sizes.push_back({w, h});
            totalWidth += w + 5;
        }

        int xStart = offsetX - totalWidth - 5;
        for (size_t i = 0; i < clues.size(); ++i) {
            int y = offsetY + row * cellSize + (cellSize - sizes[i].y) / 2;
            renderOneClue(clues[i], xStart, y);
            xStart += sizes[i].x + 5;
        }
    }

    // Column clues
    for (int col = 0; col < gridSize; ++col) {
        int yStart = offsetY - (int)colClues[col].size() * clueSpacing - 5;
        for (size_t i = 0; i < colClues[col].size(); ++i) {
            std::string clueStr = std::to_string(colClues[col][i]);
            int x = offsetX + col * cellSize;
            TTF_Font* font = getFittingFont(clueStr, cellSize, 16);
            int w = 0, h = 0;
            TTF_SizeText(font ? font : fontClue, clueStr.c_str(), &w, &h);
            if (font) TTF_CloseFont(font);

            int drawX = x + (cellSize - w) / 2;
            int drawY = yStart + i * clueSpacing;
            renderOneClue(clueStr, drawX, drawY);
        }
    }
}

TTF_Font* Gameplay::getFittingFont(const std::string& text, int maxWidth, int initialSize) {
    TTF_Font* tryFont = nullptr;
    int fontSize = initialSize;

    while (fontSize > 6) {
        tryFont = TTF_OpenFont("assets/Roboto.ttf", fontSize);

        int w = 0, h = 0;
        TTF_SizeText(tryFont, text.c_str(), &w, &h);
        if (w <= maxWidth) return tryFont;

        TTF_CloseFont(tryFont);
        fontSize--;
    }

    return nullptr;
}
