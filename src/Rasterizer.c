#include "Rasterizer.h"

#include <math.h>
#include <stdlib.h>

static void rasterizer_draw_triangle_vertices(
    Framebuffer* framebuffer,
    vec2 a,
    vec2 b,
    vec2 c,
    uint32_t color
) {
    framebuffer_set(
        framebuffer,
        (int)a.x,
        (int)a.y,
        color
    );

    framebuffer_set(
        framebuffer,
        (int)b.x,
        (int)b.y,
        color
    );

    framebuffer_set(
        framebuffer,
        (int)c.x,
        (int)c.y,
        color
    );
}

static void rasterizer_draw_bresenham_line(
    Framebuffer* framebuffer,
    ScreenVertex p0,
    ScreenVertex p1,
    uint32_t color
) {
    int x0 = (int)p0.x;
    int y0 = (int)p0.y;

    int x1 = (int)p1.x;
    int y1 = (int)p1.y;

    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int error = dx + dy;

    while (1) {
        framebuffer_set(
            framebuffer,
            x0,
            y0,
            color
        );

        if (x0 == x1 && y0 == y1) {
            break;
        }

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

static float edge(ScreenVertex a, ScreenVertex b, float x, float y) {
    return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
}

static int min3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

static int max3(int a, int b, int c) {
    int m = a > b ? a : b;
    return m > c ? m : c;
}

static void rasterizer_draw_triangle_wireframe(
    Framebuffer* framebuffer,
    ScreenVertex a,
    ScreenVertex b,
    ScreenVertex c,
    uint32_t color
) {
    rasterizer_draw_bresenham_line(framebuffer, a, b, color);
    rasterizer_draw_bresenham_line(framebuffer, b, c, color);
    rasterizer_draw_bresenham_line(framebuffer, c, a, color);
}


static void rasterizer_draw_triangle_filled(
    Framebuffer *framebuffer,
    ScreenVertex a,
    ScreenVertex b,
    ScreenVertex c,
    uint32_t color
) {
    int min_x = min3((int)floorf(a.x), (int)floorf(b.x), (int)floorf(c.x));
    int max_x = max3((int)ceilf(a.x), (int)ceilf(b.x), (int)ceilf(c.x));
    int min_y = min3((int)floorf(a.y), (int)floorf(b.y), (int)floorf(c.y));
    int max_y = max3((int)ceilf(a.y), (int)ceilf(b.y), (int)ceilf(c.y));

    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= framebuffer->width) max_x = framebuffer->width - 1;
    if (max_y >= framebuffer->height) max_y = framebuffer->height - 1;

    float area = edge(a, b, c.x, c.y);
    if (area == 0.0f) {
        return;
    }

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            float px = (float)x + 0.5f;
            float py = (float)y + 0.5f;

            float w0 = edge(b, c, px, py);
            float w1 = edge(c, a, px, py);
            float w2 = edge(a, b, px, py);

            int inside = area > 0.0f
                ? (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
                : (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f);

            if (inside) {
                framebuffer_set(framebuffer, x, y, color);
            }
        }
    }
}

void rasterizer_draw_mesh(
    Framebuffer* framebuffer,
    const ScreenMesh* mesh,
    uint32_t color,
    RasterizerMode mode
) {
    if (!framebuffer || !mesh || !mesh->vertices || !mesh->indices) {
        return;
    }

    for (uint32_t i = 0; i + 2 < mesh->index_count; i += 3) {
        uint32_t i0 = mesh->indices[i];
        uint32_t i1 = mesh->indices[i + 1];
        uint32_t i2 = mesh->indices[i + 2];

        if (
            i0 >= mesh->vertex_count ||
            i1 >= mesh->vertex_count ||
            i2 >= mesh->vertex_count
        ) {
            continue;
        }

        ScreenVertex a = mesh->vertices[i0];
        ScreenVertex b = mesh->vertices[i1];
        ScreenVertex c = mesh->vertices[i2];

        if (mode == RASTERIZER_WIREFRAME) {
            rasterizer_draw_triangle_wireframe(
                framebuffer,
                a,
                b,
                c,
                color
            );
        } else {
            rasterizer_draw_triangle_filled(
                framebuffer,
                a,
                b,
                c,
                color
            );
        }
    }
}
