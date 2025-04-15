#include "menu.h"
#include "game.h"
#include "gameplay.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <sstream>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), showRules(false) {
    TTF_Init();

    fontLarge = TTF_OpenFont("assets/MagicBubble.ttf", 90);
    fontMedium = TTF_OpenFont("assets/MagicBubble.ttf", 45);
    fontSmall = TTF_OpenFont("assets/MagicBubble.ttf", 35);

    SDL_Surface* bgSurface = IMG_Load("assets/Background(2).png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    SDL_Surface* paperSurface = IMG_Load("assets/paper.png");
    paperTexture = SDL_CreateTextureFromSurface(renderer, paperSurface);
    SDL_FreeSurface(paperSurface);

    clickSound = Mix_LoadWAV("assets/click.wav");
    Mix_VolumeChunk(clickSound, 50);
}

Menu::~Menu() {
    TTF_CloseFont(fontLarge);
    TTF_CloseFont(fontMedium);
    TTF_CloseFont(fontSmall);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(paperTexture);
    TTF_Quit();

    if (clickSound) {
        Mix_FreeChunk(clickSound);
        clickSound = nullptr;
    }
}

void Menu::render() {
    if (gameplay) {
        gameplay->render();
        return;
    }

    if (showRules) {
        renderRules();
        return;
    }

    //Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    //"Nonogram"
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface1 = TTF_RenderText_Blended(fontLarge, "Nonogram", textColor);
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Rect renderQuad1 = {210, 75, textSurface1->w, textSurface1->h};
    SDL_RenderCopy(renderer, textTexture1, NULL, &renderQuad1);
    SDL_FreeSurface(textSurface1);
    SDL_DestroyTexture(textTexture1);

    //"Pick your puzzle!"
    SDL_Surface* textSurface2 = TTF_RenderText_Blended(fontMedium, "Pick your puzzle!", textColor);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Rect renderQuad2 = {245, 200, textSurface2->w, textSurface2->h};
    SDL_RenderCopy(renderer, textTexture2, NULL, &renderQuad2);
    SDL_FreeSurface(textSurface2);
    SDL_DestroyTexture(textTexture2);

    //10x10
    SDL_Surface* buttonSurface1 = TTF_RenderText_Blended(fontSmall, "10x10", textColor);
    SDL_Texture* buttonTexture1 = SDL_CreateTextureFromSurface(renderer, buttonSurface1);
    SDL_Rect buttonRect1 = {350, 280, buttonSurface1->w, buttonSurface1->h};
    SDL_RenderCopy(renderer, buttonTexture1, NULL, &buttonRect1);
    SDL_FreeSurface(buttonSurface1);
    SDL_DestroyTexture(buttonTexture1);

    //15x15
    SDL_Surface* buttonSurface2 = TTF_RenderText_Blended(fontSmall, "15x15", textColor);
    SDL_Texture* buttonTexture2 = SDL_CreateTextureFromSurface(renderer, buttonSurface2);
    SDL_Rect buttonRect2 = {350, 340, buttonSurface2->w, buttonSurface2->h};
    SDL_RenderCopy(renderer, buttonTexture2, NULL, &buttonRect2);
    SDL_FreeSurface(buttonSurface2);
    SDL_DestroyTexture(buttonTexture2);

    //20x20
    SDL_Surface* buttonSurface3 = TTF_RenderText_Blended(fontSmall, "20x20", textColor);
    SDL_Texture* buttonTexture3 = SDL_CreateTextureFromSurface(renderer, buttonSurface3);
    SDL_Rect buttonRect3 = {340, 400, buttonSurface3->w, buttonSurface3->h};
    SDL_RenderCopy(renderer, buttonTexture3, NULL, &buttonRect3);
    SDL_FreeSurface(buttonSurface3);
    SDL_DestroyTexture(buttonTexture3);

    //"Rule"
    SDL_Surface* buttonSurface4 = TTF_RenderText_Blended(fontSmall, "Rule", textColor);
    SDL_Texture* buttonTexture4 = SDL_CreateTextureFromSurface(renderer, buttonSurface4);
    SDL_Rect buttonRect4 = {500, 500, buttonSurface4->w, buttonSurface4->h};
    SDL_RenderCopy(renderer, buttonTexture4, NULL, &buttonRect4);
    SDL_FreeSurface(buttonSurface4);
    SDL_DestroyTexture(buttonTexture4);
}

bool Menu::isButtonClicked(int x, int y, int buttonX, int buttonY, int width, int height) {
    return (x >= buttonX && x <= buttonX + width && y >= buttonY && y <= buttonY + height);
}

void Menu::handleEvents(SDL_Event& e) {
    if (gameplay) {
        if (gameplay->isLose()) return;
        gameplay->handleEvents(e);
        return;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (isButtonClicked(x, y, 350, 280, 150, 50)) {
            Mix_PlayChannel(-1, clickSound, 0);
            startGame(10);
        } else if (isButtonClicked(x, y, 350, 340, 150, 50)) {
            Mix_PlayChannel(-1, clickSound, 0);
            startGame(15);
        } else if (isButtonClicked(x, y, 340, 400, 150, 50)) {
            Mix_PlayChannel(-1, clickSound, 0);
            startGame(20);
        } else if (isButtonClicked(x, y, 500, 500, 100, 50)) {
            Mix_PlayChannel(-1, clickSound, 0);
            showRules = !showRules;
        }
    }
}

void Menu::renderRules() {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_Rect paperRect = {100, 10, 600, 600};
    SDL_RenderCopy(renderer, paperTexture, NULL, &paperRect);
}

void Menu::startGame(int size) {
    boardSize = size;
    isPlaying = true;

    gameplay = new Gameplay(renderer, size, fontSmall);
}
