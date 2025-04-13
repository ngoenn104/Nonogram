#include "status.h"
#include "game.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    TTF_Font* font = TTF_OpenFont("assets/MagicBubble.ttf", fontSize);

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst;
    dst.x = x - surface->w / 2;
    dst.y = y - surface->h / 2;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void renderBackground(SDL_Renderer* renderer, const std::string& imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dst = {0, 0, 800, 600};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void renderStatus(SDL_Renderer* renderer, bool isWin, bool isLose, int lives) {
    if (isWin || isLose) {
        renderBackground(renderer, "assets/Background(2).png");

        SDL_Color color = isWin ? SDL_Color{0, 255, 0} : SDL_Color{255, 0, 0};
        std::string text = isWin ? "YOU WIN" : "YOU LOSE";

        renderText(renderer, text, 400, 300, color, 64);
    }

    else {
        std::string livesText = "Lives " + std::to_string(lives);
        SDL_Color black = {0, 0, 0};
        renderText(renderer, livesText, 700, 40, black, 30);
    }
}
