#pragma once

#include <SDL3/SDL.h>

#include "Framebuffer.h"

typedef struct Display{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

int display_create(
    Display* display,
    int width,
    int height
);

void display_destroy(
    Display* display
);

void display_present(
    Display* display,
    const Framebuffer* framebuffer
);
