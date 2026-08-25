#pragma once

#include <stdint.h>

typedef struct Framebuffer{
    uint32_t* pixels;
    int width;
    int height;
};

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
