// keyboard.hpp
#pragma once
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include <SDL2/SDL.h>
#include "global.hpp"
inline void keyboardInputs(double deltaTime) {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    static bool f1WasDown = false;

    float sinYaw = sinf(camYaw);
    float cosYaw = cosf(camYaw);

    float forwardX =  sinYaw;
    float forwardZ = -cosYaw;

    float rightX   =  cosYaw;
    float rightZ   =  sinYaw;

    if (keystate[SDL_SCANCODE_F1]) {
        if (!f1WasDown)
            debugMode = !debugMode;
        f1WasDown = true;
    } else {
        f1WasDown = false;
    }/*
    if (keystate[SDL_SCANCODE_F11]) {
        SDL_SetWindowFullscreen(
            window,
            SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN
                ? 0
                : SDL_WINDOW_FULLSCREEN
        );
    }*/
    if (keystate[SDL_SCANCODE_W]) {
        camX += forwardX * moveSpeed * deltaTime;
        camZ += forwardZ * moveSpeed * deltaTime;
    }
    if (keystate[SDL_SCANCODE_S]) {
        camX -= forwardX * moveSpeed * deltaTime;
        camZ -= forwardZ * moveSpeed * deltaTime;
    }
    if (keystate[SDL_SCANCODE_A]) {
        camX -= rightX * moveSpeed * deltaTime;
        camZ -= rightZ * moveSpeed * deltaTime;
    }
    if (keystate[SDL_SCANCODE_D]) {
        camX += rightX * moveSpeed * deltaTime;
        camZ += rightZ * moveSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_F8]) engineRunning = false;
}

#endif // KEYBOARD_HPP