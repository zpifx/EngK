#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include "renderer.hpp" // setPixel()
class TextRenderer {
public:
    TTF_Font* font;

    TextRenderer(const char* fontPath, int fontSize) {
        font = TTF_OpenFont(fontPath, fontSize);
        if (!font) {
            std::cerr << "Font load failed: " << TTF_GetError() << std::endl;
            std::exit(1);
        }
    }

    ~TextRenderer() {
        TTF_CloseFont(font);
    }

    void drawText(
        const std::string& text,
        int x, int y,
        SDL_Color color
    ) {
        SDL_Surface* surface =
            TTF_RenderText_Blended(font, text.c_str(), color);

        if (!surface) {
            std::cerr << "Text render failed: " << TTF_GetError() << std::endl;
            return;
        }

        // Surface must be ARGB8888
        SDL_Surface* formatted =
            SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(surface);

        uint32_t* pixels = (uint32_t*)formatted->pixels;

        for (int py = 0; py < formatted->h; ++py) {
            for (int px = 0; px < formatted->w; ++px) {
                uint32_t pixel = pixels[py * formatted->w + px];

                uint8_t a = (pixel >> 24) & 0xFF;
                if (a == 0) continue; // skip transparent

                uint8_t r = (pixel >> 16) & 0xFF;
                uint8_t g = (pixel >> 8) & 0xFF;
                uint8_t b = pixel & 0xFF;

                setPixel(x + px, y + py, a, r, g, b);
            }
        }

        SDL_FreeSurface(formatted);
    }
};
struct TextItem {
    std::string text;
    int x, y;
    SDL_Color color;
};
