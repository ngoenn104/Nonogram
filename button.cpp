#include "button.h"
#include <SDL_ttf.h>
#include <SDL.h>

Button::Button(SDL_Renderer* ren, TTF_Font* f) {
    renderer = ren;
    font = f;
    rect = {0, 0, 100, 50};
    textColor = {255, 255, 255}; // trắng
    bgColor = {0, 0, 0}; // đen
    texture = nullptr;
}

void Button::setText(const std::string& txt) {
    text = txt;

    // xoá texture cũ nếu có
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Button::setRect(int x, int y, int w, int h) {
    rect = {x, y, w, h};
}

void Button::render() {
    // vẽ nền
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
    SDL_RenderFillRect(renderer, &rect);

    // vẽ viền trắng
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // vẽ chữ nếu có
    if (texture) {
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        SDL_Rect dst = {
            rect.x + (rect.w - tw) / 2,
            rect.y + (rect.h - th) / 2,
            tw, th
        };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }
}

bool Button::isHover(int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void Button::handleClick(int x, int y, std::function<void()> onClick) {
    if (isHover(x, y)) {
        onClick(); // gọi hành động khi click
    }
}
