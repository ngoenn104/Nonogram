#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>

class Button {
public:
    Button(SDL_Renderer* ren, TTF_Font* f); // khởi tạo đơn giản

    void setText(const std::string& txt);   // đặt chữ
    void setRect(int x, int y, int w, int h); // đặt vị trí, kích thước
    void render();                          // vẽ nút
    bool isHover(int x, int y);             // kiểm tra chuột đè lên chưa
    void handleClick(int x, int y, std::function<void()> onClick); // xử lý click

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Rect rect;
    SDL_Color textColor;
    SDL_Color bgColor;
    std::string text;
    SDL_Texture* texture;
};

#endif
