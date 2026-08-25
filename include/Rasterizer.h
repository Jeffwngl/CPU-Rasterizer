#pragma once

#include "Framebuffer.h"
#include "Transform.h"

typedef enum {
    RASTERIZER_WIREFRAME,
    RASTERIZER_FILLED
} RasterizerMode;

void rasterizer_draw_mesh(
    Framebuffer* frameBuffer,
    const ScreenMesh *mesh,
    uint32_t color,
    RasterizerMode mode
);
