#ifndef GAME_H
#define GAME_H

#include <vector>

class Game {
public:
    Game(int size);
    std::vector<std::vector<int>> getMatrix() const;
    std::vector<std::vector<int>> getRowClues() const;
    std::vector<std::vector<int>> getColClues() const;

private:
    int size;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> rowClues;
    std::vector<std::vector<int>> colClues;

    void genMatrix();
    void genClues();
    std::vector<int> genClueForLine(const std::vector<int>& line);
};

#endif

