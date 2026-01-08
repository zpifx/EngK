// renderer.cpp
#include "renderer.hpp"
#include "project.hpp"
#include <vector>

void drawTestPattern() {
    // Draw a red diagonal line from top-left to bottom-right
    drawLine(0, 0, FB_WIDTH - 1, FB_HEIGHT - 1, 0xFF, 0xFF, 0x00, 0x00);

    // Draw a white pixel in the center
    setPixel(FB_WIDTH / 2, FB_HEIGHT / 2, 0xFF, 0xFF, 0xFF, 0xFF);

    drawRectangle(100, 100, 200, 150, 0xFF, 0x00, 0xFF, 0x00); // Green rectangle
    drawTriangle(400, 100, 550, 300, 250, 300, 0xFF, 0x00, 0x00, 0xFF); // Blue triangle
}

Vec3 intersectNear(const Vec3& a, const Vec3& b)
{
    float t = (-NEAR_PLANE - a.z) / (b.z - a.z);

    return {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        -NEAR_PLANE
    };
}

int clipTriangleNear(
    const Vec3& v0, const Vec3& v1, const Vec3& v2,
    Vec3 out[4]
)
{
    const Vec3* in[3] = { &v0, &v1, &v2 };
    bool inside[3] = {
        insideNear(v0),
        insideNear(v1),
        insideNear(v2)
    };

    int count = 0;

    for (int i = 0; i < 3; i++) {
        const Vec3& curr = *in[i];
        const Vec3& next = *in[(i + 1) % 3];

        bool currIn = inside[i];
        bool nextIn = inside[(i + 1) % 3];

        if (currIn)
            out[count++] = curr;

        if (currIn != nextIn)
            out[count++] = intersectNear(curr, next);
    }

    return count;
}

void drawProjectedTriangle(const Vec3& a, const Vec3& b, const Vec3& c)
{
    Vec2i sa, sb, sc;

    if (!projectVertex(a, sa)) return;
    if (!projectVertex(b, sb)) return;
    if (!projectVertex(c, sc)) return;

    drawTriangle(
        sa.x, sa.y,
        sb.x, sb.y,
        sc.x, sc.y,
        255, 0, 255, 0
    );
}


void renderFrame(Polygon *World, size_t polyCount, Camera& cam) {
    clearFramebuffer(0xFF000000); // Clear to opaque black

    for (size_t i = 0; i < polyCount; ++i) {
        Vec3 p0 = worldToView(World[i].v0, cam);
        Vec3 p1 = worldToView(World[i].v1, cam);
        Vec3 p2 = worldToView(World[i].v2, cam);
/*
        // Backface culling
        Vec3 a { p1.x - p0.x, p1.y - p0.y, p1.z - p0.z };
        Vec3 b { p2.x - p0.x, p2.y - p0.y, p2.z - p0.z };

        float nz = a.x * b.y - a.y * b.x;
        if (nz >= 0.0f)
            continue;
*/
        drawProjectedTriangle(p0, p1, p2);
    }

}