// main.cpp
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <vector>

#include "global.hpp"
#include "renderer.hpp"
#include "project.hpp"
#include "keyboard.hpp"
#include "renderText.hpp"

uint64_t perfFreq = SDL_GetPerformanceFrequency();
uint64_t lastCounter = SDL_GetPerformanceCounter();

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Allocate software framebuffer
    framebuffer = new uint32_t[FB_WIDTH * FB_HEIGHT];

    // Create window (720p)
    SDL_Window* window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create renderer (SOFTWARE or ACCELERATED both fine)
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_SOFTWARE
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create streaming texture for framebuffer
    SDL_Texture* fbTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        FB_WIDTH,
        FB_HEIGHT
    );

    if (!fbTexture) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Critical for pixel-perfect output
    SDL_SetTextureBlendMode(fbTexture, SDL_BLENDMODE_NONE);
    SDL_SetTextureScaleMode(fbTexture, SDL_ScaleModeNearest);

    TextRenderer textRenderer("font.ttf", 8);
    std::vector<TextItem> texts;
    Camera cam { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    WorldMesh World;
    static Polygon worldPolys[] = {
        // -------- FRONT (-Z) --------
        { { -1,  1, -9 }, {  1,  1, -9 }, { -1, -1, -9 } },
        { {  1, -1, -9 }, {  1,  1, -9 }, { -1, -1, -9 } },

        // -------- BACK (+Z) --------
        { {  1,  1, -11 }, { -1,  1, -11 }, {  1, -1, -11 } },
        { { -1, -1, -11 }, { -1,  1, -11 }, {  1, -1, -11 } },

        // -------- LEFT (-X) --------
        { { -1,  1, -11 }, { -1,  1, -9 }, { -1, -1, -11 } },
        { { -1, -1, -9 }, { -1, -1, -11 }, { -1,  1, -9 } },

        // -------- RIGHT (+X) --------
        { {  1,  1, -9 }, {  1,  1, -11 }, {  1, -1, -9 } },
        { {  1, -1, -11 }, {  1, -1, -9 }, {  1,  1, -11 } },

        // -------- TOP (+Y) --------
        { { -1,  1, -11 }, {  1,  1, -11 }, { -1,  1, -9 } },
        { {  1,  1, -9 }, { -1,  1, -9 }, {  1,  1, -11 } },

        // -------- BOTTOM (-Y) --------
        { { -1, -1, -9 }, {  1, -1, -9 }, { -1, -1, -11 } },
        { {  1, -1, -11 }, { -1, -1, -11 }, {  1, -1, -9 } }
    };


    World.polygons = worldPolys;
    World.polygonCount = sizeof(worldPolys) / sizeof(Polygon);


    SDL_Event event;
    while (engineRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                engineRunning = false;
                break;
            default:
                break;
            }
        }

        uint64_t current = SDL_GetPerformanceCounter();
        double deltaTime =
            (double)(current - lastCounter) / (double)perfFreq;
        lastCounter = current;

        int mouseDX, mouseDY;
        SDL_GetRelativeMouseState(&mouseDX, &mouseDY);
        camYaw += mouseDX * turnSpeed * deltaTime;
        camPitch += mouseDY * turnSpeed * deltaTime;

        // FPS smoothing
        static double fpsTimer = 0.0;
        static int fpsFrames = 0;
        static double fpsSmoothed = 0.0;

        fpsTimer += deltaTime;
        fpsFrames++;

        if (fpsTimer >= 0.5) {
            fpsSmoothed = fpsFrames / fpsTimer;
            fpsFrames = 0;
            fpsTimer = 0.0;
        }

        if (camPitch >  1.57f) camPitch =  1.57f;
        if (camPitch < -1.57f) camPitch = -1.57f;

        keyboardInputs(deltaTime);
        cam = { camX, camY, camZ, camYaw, camPitch };
        renderFrame(World.polygons, World.polygonCount, cam);
        if (debugMode) {
            char *debugText = new char[128];
            snprintf(
                debugText,
                128,
                "Debug Text | CamX: %.2f CamY: %.2f CamZ: %.2f Yaw: %.2f Pitch: %.2f FPS: %.2f",
                camX, camY, camZ, camYaw, camPitch, fpsSmoothed
            );
            texts.push_back({debugText, 10, 20, {255,0,0,255}});
        }
        texts.push_back({"EngK v" VERSION_STRING, 10, 10, {255,255,255,255}});

        for (const auto& t : texts) {
            textRenderer.drawText(t.text, t.x, t.y, t.color);
        }

        // Upload framebuffer to texture
        SDL_UpdateTexture(
            fbTexture,
            nullptr,
            framebuffer,
            FB_WIDTH * sizeof(uint32_t)
        );

        // ----- PRESENT -----
        SDL_RenderClear(renderer);

        SDL_Rect dstRect {
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        };

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fbTexture, nullptr, &dstRect);
        SDL_RenderPresent(renderer);

        double targetFrame = 1.0 / 60.0;
        if (deltaTime < targetFrame) {
            SDL_Delay((Uint32)((targetFrame - deltaTime) * 1000.0));
        }
        texts.clear();
    }

    // Cleanup
    delete[] framebuffer;
    SDL_DestroyTexture(fbTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
