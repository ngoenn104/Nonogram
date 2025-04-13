#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class Gameplay {
public:
    Gameplay(SDL_Renderer* renderer, int gridSize, TTF_Font* font);
    ~Gameplay();

    void handleCellClick(SDL_Event e, int mouseX, int mouseY);
    void handleEvents(SDL_Event& e);
    void update();
    void render();

    bool gameOver;
    bool win;
    bool isLose() const;

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* fontClue;
    int gridSize;
    int cellSize;
    int offsetX;
    int offsetY;
    int lives;

    std::vector<std::vector<int>> solution;
    std::vector<std::vector<int>> playerMatrix;
    std::vector<std::vector<int>> rowClues;
    std::vector<std::vector<int>> colClues;

    void renderGrid();
    void renderClues();
    void checkWin();

    TTF_Font* getFittingFont(const std::string& text, int maxWidth, int initialSize);
};

#endif
