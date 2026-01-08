// project.cpp
#include "project.hpp"
#include <cmath>

static float g_projScale = 1.0f;
static float g_aspect    = (float)FB_WIDTH / (float)FB_HEIGHT;

void setFOV(float degrees)
{
    float rad = degrees * 0.5f * (3.14159265f / 180.0f);
    g_projScale = 1.0f / tanf(rad);
}

Vec3 worldToView(const Vec3& v, const Camera& cam)
{
    float x = v.x - cam.x;
    float y = v.y - cam.y;
    float z = v.z - cam.z;

    float cy = cosf(cam.yaw);
    float sy = sinf(cam.yaw);

    float xz =  cy * x + sy * z;
    float zz = -sy * x + cy * z;

    float cp = cosf(cam.pitch);
    float sp = sinf(cam.pitch);

    float yz =  cp * y - sp * zz;
    float zz2 = sp * y + cp * zz;

    return {
        xz,
        yz,
        zz2
    };
}

bool projectVertex(const Vec3& v, Vec2i& outScreenPos) {
    if (isBehindNearPlane(v.z)) return false;

    outScreenPos = projectToScreen(
        v.x,
        v.y,
        v.z,
        g_projScale,
        g_aspect
    );
    return true;
}
