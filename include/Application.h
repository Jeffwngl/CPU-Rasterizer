#pragma once

#include "Display.h"
#include "Framebuffer.h"
#include "Model.h"
#include "Rasterizer.h"
#include "Transform.h"

typedef struct {
    Display display;
    Framebuffer framebuffer;

    Mesh mesh;
    ScreenMesh screen_mesh;

    mat4 model;
    mat4 view;
    mat4 projection;

    RasterizerMode rasterizer_mode;
    uint32_t clear_color;
    uint32_t mesh_color;

    int width;
    int height;
    int running;
} Application;

int application_create(
    Application *application,
    const char *model_path,
    int width,
    int height
);

void application_run(Application *application);
void application_destroy(Application *application);

void application_set_rasterizer_mode(
    Application *application,
    RasterizerMode mode
);
