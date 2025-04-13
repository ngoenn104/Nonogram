#include "grid.h"
#include <vector>
#include <cstdlib>
#include <ctime>

renderGrid::renderGrid(SDL_Renderer* ren, int n, int box, int x, int y)
    : renderer(ren), size(n), boxSize(box), padX(x), padY(y) {}

void renderGrid::draw(const std::vector<std::vector<int>>& playerMat) {
    //background
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            SDL_Rect cell = { padX + j * boxSize, padY + i * boxSize, boxSize, boxSize };
            SDL_RenderFillRect(renderer, &cell);
        }

    //player's marks
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            SDL_Rect cell = { padX + j * boxSize + 1, padY + i * boxSize + 1, boxSize - 2, boxSize - 2 };
            if (playerMat[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &cell);
            } else if (playerMat[i][j] == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, cell.x, cell.y, cell.x + cell.w, cell.y + cell.h);
                SDL_RenderDrawLine(renderer, cell.x + cell.w, cell.y, cell.x, cell.y + cell.h);
            }
        }

    //light grid
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    for (int i = 0; i <= size; ++i) {
        SDL_RenderDrawLine(renderer, padX, padY + i * boxSize, padX + size * boxSize, padY + i * boxSize);
        SDL_RenderDrawLine(renderer, padX + i * boxSize, padY, padX + i * boxSize, padY + size * boxSize);
    }

    //thick lines every 5
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (int i = 0; i <= size; ++i)
        if (i % 5 == 0) {
            SDL_RenderDrawLine(renderer, padX, padY + i * boxSize, padX + size * boxSize, padY + i * boxSize);
            SDL_RenderDrawLine(renderer, padX + i * boxSize, padY, padX + i * boxSize, padY + size * boxSize);
        }
}

std::vector<std::vector<int>> makeGrid(int n) {
    std::srand(std::time(nullptr));
    std::vector<std::vector<int>> mat(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mat[i][j] = std::rand() % 2;
    return mat;
}
