#pragma once

#include <stdint.h>

typedef struct {
    uint32_t* pixels;
    int width;
    int height;
} Framebuffer;

int framebuffer_create(
    Framebuffer* framebuffer,
    int width,
    int height
);

void framebuffer_destroy(
    Framebuffer* framebuffer
);

void framebuffer_set(
    Framebuffer* framebuffer,
    int x,
    int y,
    uint32_t color
);

void framebuffer_clear(
    Framebuffer *framebuffer,
    uint32_t color
);
