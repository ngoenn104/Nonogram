#include "status.h"
#include "game.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

Mix_Chunk* winSound = nullptr;
Mix_Chunk* loseSound = nullptr;
bool hasPlayedSound = false;

void initStatusSounds() {
    winSound = Mix_LoadWAV("assets/win.wav");
    loseSound = Mix_LoadWAV("assets/lose.wav");
    Mix_VolumeChunk(winSound, 128);
    Mix_VolumeChunk(loseSound, 60);
}

void cleanupStatusSounds() {
    Mix_FreeChunk(winSound);
    Mix_FreeChunk(loseSound);
    winSound = nullptr;
    loseSound = nullptr;
    hasPlayedSound = false;
}

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    TTF_Font* font = TTF_OpenFont("assets/PressStart.ttf", fontSize);

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
        renderBackground(renderer, "assets/Bg.png");

        if (!hasPlayedSound) {
            if (isWin && winSound) Mix_PlayChannel(-1, winSound, 0);
            if (isLose && loseSound) Mix_PlayChannel(-1, loseSound, 0);
            hasPlayedSound = true;
        }

        SDL_Color color = isWin ? SDL_Color{77, 108, 168, 255} : SDL_Color{77, 108, 168, 255};
        std::string text = isWin ? "YOU WIN" : "YOU LOSE";

        renderText(renderer, text, 400, 300, color, 70);
    } else {
        hasPlayedSound = false;
    }
}
