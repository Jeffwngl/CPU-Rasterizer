#include "Framebuffer.h"

#include <stdlib.h>

int framebuffer_create(
    Framebuffer* framebuffer,
    int width,
    int height
) {
    framebuffer->width = width;
    framebuffer->height = height;

    framebuffer->pixels = malloc(width * height * sizeof(uint32_t));

    return framebuffer->pixels != NULL;
}

void framebuffer_destroy(
    Framebuffer* framebuffer
) {
    free(framebuffer->pixels);

    framebuffer->pixels = NULL;
    framebuffer->width = 0;
    framebuffer->height = 0;
}

void framebuffer_clear(
    Framebuffer* framebuffer,
    uint32_t color
) {
    int cnt = framebuffer->width * framebuffer->height;

    for (int i = 0; i < cnt; ++i) {
        framebuffer->pixels[i] = color;
    }
}

void framebuffer_set(
    Framebuffer* framebuffer,
    int x,
    int y,
    uint32_t color
) {
    if (framebuffer->pixels == NULL) {
        return;
    }

    if (x < 0 ||
        x >= framebuffer->width ||
        y < 0 ||
        y >= framebuffer->height
    ) {
        return;
    }

    int loc = framebuffer->width * y + x;

    framebuffer->pixels[loc] = color;
}
