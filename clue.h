#ifndef CLUE_H
#define CLUE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class ClueDrawer {
public:
    ClueDrawer(SDL_Renderer* ren, TTF_Font* fallbackFont, int n, int boxSize, int padX, int padY);
    void draw(const std::vector<std::vector<int>>& rowClue, const std::vector<std::vector<int>>& colClue);
    static TTF_Font* getFitFont(const std::string& text, int maxWidth, int startSize = 16);

private:
    SDL_Renderer* renderer;
    TTF_Font* fontDefault;
    int size;
    int boxSize;
    int padX, padY;
};

#endif

