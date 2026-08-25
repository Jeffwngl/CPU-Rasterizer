#include "Display.h"

int display_create(
    Display* display,
    int width,
    int height
) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }

    display->window = SDL_CreateWindow(
        "CPU Rasterizer",
        width,
        height,
        0
    );

    if (!display->window) {
        return 0;
    }

    display->renderer = SDL_CreateRenderer(
        display->window,
        NULL
    );

    if (!display->renderer) {
        return 0;
    }

    display->texture = SDL_CreateTexture(
        display->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if (!display->texture) {
        return 0;
    }

    return 1;
}

void display_destroy(
    Display* display
) {
    SDL_DestroyTexture(display->texture);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);

    SDL_Quit();
}

void display_present(
    Display* display,
    const Framebuffer* framebuffer
) {
    SDL_UpdateTexture(
        display->texture,
        NULL,
        framebuffer->pixels,
        framebuffer->width * sizeof(uint32_t)
    );

    SDL_RenderClear(display->renderer);

    SDL_RenderTexture(
        display->renderer,
        display->texture,
        NULL,
        NULL
    );

    SDL_RenderPresent(display->renderer);
}
