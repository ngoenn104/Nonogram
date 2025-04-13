#ifndef GRID_H
#define GRID_H

#include <SDL.h>
#include <vector>

std::vector<std::vector<int>> makeGrid(int n);

class renderGrid {
public:
    renderGrid(SDL_Renderer* ren, int n, int boxSize, int padX, int padY);
    void draw(const std::vector<std::vector<int>>& playerMat);

private:
    SDL_Renderer* renderer;
    int size;
    int boxSize;
    int padX, padY;
};

#endif

