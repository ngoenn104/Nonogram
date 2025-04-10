#include "clue.h"
#include <SDL_ttf.h>

ClueDrawer::ClueDrawer(SDL_Renderer* ren, TTF_Font* fallbackFont, int n, int box, int x, int y)
    : renderer(ren), fontDefault(fallbackFont), size(n), boxSize(box), padX(x), padY(y) {}

void ClueDrawer::draw(const std::vector<std::vector<int>>& rowClue, const std::vector<std::vector<int>>& colClue) {
    SDL_Color textColor = { 0, 0, 0, 255 };
    const int clueSpacing = 16;

    //Row clues
    for (int i = 0; i < size; ++i) {
        int totalWidth = 0;
        std::vector<SDL_Texture*> textures;
        std::vector<SDL_Rect> rects;

        for (int clue : rowClue[i]) {
            std::string clueStr = std::to_string(clue);
            TTF_Font* fitFont = getFitFont(clueStr, boxSize);
            SDL_Surface* surface = TTF_RenderText_Blended(fitFont ? fitFont : fontDefault, clueStr.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect rect = { 0, 0, surface->w, surface->h };
            textures.push_back(texture);
            rects.push_back(rect);

            totalWidth += surface->w + 5;

            SDL_FreeSurface(surface);
            if (fitFont) TTF_CloseFont(fitFont);
        }

        int xStart = padX - totalWidth - 5;
        for (size_t k = 0; k < textures.size(); ++k) {
            SDL_Rect dst = {
                xStart,
                padY + i * boxSize + (boxSize - rects[k].h) / 2,
                rects[k].w,
                rects[k].h
            };
            SDL_RenderCopy(renderer, textures[k], nullptr, &dst);
            xStart += rects[k].w + 5;
            SDL_DestroyTexture(textures[k]);
        }
    }

    //Column clues
    for (int j = 0; j < size; ++j) {
        int yStart = padY - static_cast<int>(colClue[j].size()) * clueSpacing - 5;

        for (size_t k = 0; k < colClue[j].size(); ++k) {
            std::string clueStr = std::to_string(colClue[j][k]);
            TTF_Font* fitFont = getFitFont(clueStr, boxSize);
            SDL_Surface* surface = TTF_RenderText_Blended(fitFont ? fitFont : fontDefault, clueStr.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect dstRect = {
                padX + j * boxSize + (boxSize - surface->w) / 2,
                yStart + k * clueSpacing,
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

TTF_Font* ClueDrawer::getFitFont(const std::string& text, int maxWidth, int startSize) {
    TTF_Font* tryFont = nullptr;
    int fontSize = startSize;

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

