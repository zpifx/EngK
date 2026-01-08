// global.hpp
#pragma once
#ifndef GLOBAL_HPP
#define GLOBAL_HPP
// Global header file for common includes and definitions
// Window parameters
#define WINDOW_WIDTH        1920
#define WINDOW_HEIGHT       1080
#define WINDOW_TITLE        "EngK"
#define WINDOW_FRAME_RATE   60
#define WINDOW_TIME_STEP    (1.0f / FRAME_RATE)
// General parameters
#define VERSION_STRING      "0.0.1-alpha"
// Renderer parameters
#define FB_WIDTH            960
#define FB_HEIGHT           540
#define FOV_DEGREES         90.0f
// Engine variables
inline float camX = 0.0f;
inline float camY = 0.0f;
inline float camZ = 0.0f;
inline float camYaw = 0.0f;
inline float camPitch = 0.0f;
inline bool debugMode = false;
inline bool engineRunning = true;
inline float moveSpeed = 1.0f;
inline float turnSpeed = 0.1f;
#endif // GLOBAL_HPP