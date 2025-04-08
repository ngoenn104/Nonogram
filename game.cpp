#include "game.h"
#include <cstdlib>
#include <ctime>

Game::Game(int size) : size(size) {
    srand(time(0));
    genMatrix();
    genClues();
}

std::vector<std::vector<int>> Game::getMatrix() const {
    return matrix;
}

std::vector<std::vector<int>> Game::getRowClues() const {
    return rowClues;
}

std::vector<std::vector<int>> Game::getColClues() const {
    return colClues;
}

void Game::genMatrix() {
    matrix.resize(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 2;
        }
    }
}

std::vector<int> Game::genClueForLine(const std::vector<int>& line) {
    std::vector<int> clue;
    int count = 0;
    for (int cell : line) {
        if (cell == 1) {
            count++;
        } else if (count > 0) {
            clue.push_back(count);
            count = 0;
        }
    }
    if (count > 0) {
        clue.push_back(count);
    }
    return clue;
}

void Game::genClues() {
    rowClues.clear();
    colClues.clear();

    for (const auto& row : matrix) {
        rowClues.push_back(genClueForLine(row));
    }

    for (int j = 0; j < size; ++j) {
        std::vector<int> col;
        for (int i = 0; i < size; ++i) {
            col.push_back(matrix[i][j]);
        }
        colClues.push_back(genClueForLine(col));
    }
}
