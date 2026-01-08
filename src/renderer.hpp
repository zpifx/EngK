// renderer.hpp
#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <SDL2/SDL.h>
#include <iostream>
#include "global.hpp"
#include "project.hpp"
#include <cstdint>
#include <cmath>
#include <vector>
#define NEAR_PLANE 0.5f
void drawTestPattern();
void renderFrame(Polygon *world, size_t polyCount, Camera& cam);

inline uint32_t *framebuffer = nullptr;

inline void setPixel(int x, int y, uint8_t a, uint8_t r, uint8_t g, uint8_t b){
    if (x < 0 || y < 0 || x >= FB_WIDTH || y >= FB_HEIGHT)
        return;

    framebuffer[y * FB_WIDTH + x] =
        (a << 24) |
        (r << 16) |
        (g << 8)  |
        (b);
}

inline void clearFramebuffer(uint32_t color){
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; ++i) {
        framebuffer[i] = color;
    }
}

inline void drawLine(int x0, int y0, int x1, int y1, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    
    while (true) {
        setPixel(x0, y0, a, r, g, b);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * error;
        if (e2 >= dy) {
            error += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}

inline void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    drawLine(x0, y0, x1, y1, a, r, g, b);
    drawLine(x1, y1, x2, y2, a, r, g, b);
    drawLine(x2, y2, x0, y0, a, r, g, b);
}

inline void drawRectangle(int x, int y, int width, int height, uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    drawTriangle(x, y, x + width, y, x + width, y + height, a, r, g, b);
    drawTriangle(x, y, x, y + height, x + width, y + height, a, r, g, b);
}

inline bool insideNear(const Vec3& v) {
    return v.z <= -NEAR_PLANE;
}

#endif // RENDERER_HPP