#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
public:
    Board(int size);
    void generate();                       // Tạo bảng ngẫu nhiên
    void render();                         // Hiển thị bảng (vẽ lên màn hình)
    std::vector<std::vector<int>> getBoard();  // Trả về bảng hiện tại (chứa giá trị 0 và 1)
    std::vector<std::vector<std::vector<int>>> getClues();  // Trả về gợi ý cho các hàng và cột

private:
    int size;                              // Kích thước bảng (10x10, 15x15, v.v.)
    std::vector<std::vector<int>> board;    // Ma trận bảng
    std::vector<std::vector<std::vector<int>>> clues;  // Gợi ý cho hàng và cột
    void generateClues();                  // Tạo gợi ý cho bảng
};

#endif

