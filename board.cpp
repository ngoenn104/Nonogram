#include "board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Board::Board(int size) : size(size) {
    board.resize(size, std::vector<int>(size, 0));
    generate();
}

void Board::generate() {
    srand(static_cast<unsigned>(time(0)));

    // Tạo bảng ngẫu nhiên: 0 là ô trống, 1 là ô điền
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = rand() % 2;  // 0 hoặc 1
        }
    }
    generateClues();
}

void Board::generateClues() {
    // Tạo gợi ý cho các hàng
    clues.resize(2, std::vector<std::vector<int>>(size));  // Dành cho hàng và cột

    // Tạo gợi ý cho các hàng
    for (int i = 0; i < size; ++i) {
        std::vector<int> rowClue;
        int count = 0;
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == 1) {
                count++;
            } else if (count > 0) {
                rowClue.push_back(count);
                count = 0;
            }
        }
        if (count > 0) rowClue.push_back(count);
        clues[0][i] = rowClue;  // Gợi ý cho hàng i
    }

    // Tạo gợi ý cho các cột (giống như trên)
    for (int j = 0; j < size; ++j) {
        std::vector<int> colClue;
        int count = 0;
        for (int i = 0; i < size; ++i) {
            if (board[i][j] == 1) {
                count++;
            } else if (count > 0) {
                colClue.push_back(count);
                count = 0;
            }
        }
        if (count > 0) colClue.push_back(count);
        clues[1][j] = colClue;  // Gợi ý cho cột j
    }
}

void Board::render() {
    // Vẽ bảng lên màn hình (SDL code)
    // Dùng SDL để vẽ bảng, có thể thay đổi màu sắc để phân biệt các ô điền và không điền
}

std::vector<std::vector<int>> Board::getBoard() {
    return board;
}

std::vector<std::vector<std::vector<int>>> Board::getClues() {
    return clues;
}

