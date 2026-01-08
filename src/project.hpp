// project.hpp
#pragma once
#ifndef PROJECT_HPP
#define PROJECT_HPP
#include <cstdint>
#include "global.hpp"

class Vec3 {
public:
    float x, y, z;
};

struct Vec2i {
    int x, y;
};

struct Polygon {
    Vec3 v0, v1, v2;
};

struct WorldMesh {
    Polygon* polygons;
    size_t polygonCount;
};

struct Camera {
    float x, y, z;
    float yaw, pitch;
};

inline Vec2i projectToScreen(float x, float y, float z,
                             float projScale,
                             float aspect)
{
    float invZ = -1.0f / z;

    float sx = (x * projScale / aspect) * invZ;
    float sy =  y * projScale * invZ;

    return {
        (int)((sx + 1.0f) * 0.5f * FB_WIDTH),
        (int)((1.0f - sy) * 0.5f * FB_HEIGHT)
    };
}

// Near-plane test (HOT)
inline bool isBehindNearPlane(float z) {
    return z >= -0.5f;
}

Vec3 worldToView(const Vec3& v, const Camera& cam);
bool projectVertex(const Vec3& viewPos, Vec2i& outScreenPos);
void setFOV(float degrees);
#endif // PROJECT_HPP