#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "menu.h"
#include "status.h"

int main(int argc, char* argv[]) {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    initStatusSounds();
    Mix_Music* bgm = nullptr;

    SDL_Window* window = SDL_CreateWindow("Nonogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Menu menu(renderer);

    bool isRunning = true;
    SDL_Event e;

    bgm = Mix_LoadMUS("assets/bg.mp3");
    Mix_PlayMusic(bgm, -1);
    Mix_VolumeMusic(60);

    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                isRunning = false;
            }
            menu.handleEvents(e);
        }

        SDL_RenderClear(renderer);
        menu.render();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    cleanupStatusSounds();
    Mix_CloseAudio();
    return 0;
}
