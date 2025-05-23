#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "gameplay.h"

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void render();
    void handleEvents(SDL_Event& e);
    bool isButtonClicked(int x, int y, int buttonX, int buttonY, int width, int height);

    void startGame(int size);

private:
    SDL_Renderer* renderer;
    TTF_Font* fontLarge;
    TTF_Font* fontMedium;
    TTF_Font* fontSmall;
    SDL_Texture* backgroundTexture;
    SDL_Texture* paperTexture;
    bool showRules;
    void renderRules();

    bool isPlaying = false;
    int boardSize;

    Gameplay* gameplay = nullptr;
    Mix_Chunk* clickSound = nullptr;
};

#endif
