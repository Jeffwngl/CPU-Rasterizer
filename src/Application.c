#include "Application.h"

#include <SDL3/SDL.h>

static void application_update_window_title(const Application *application) {
    const char *mode_name = application->rasterizer_mode == RASTERIZER_WIREFRAME
        ? "Wireframe"
        : "Filled";

    char title[128];
    SDL_snprintf(
        title,
        sizeof(title),
        "CPU Rasterizer - %s | W: wireframe, F: filled, space: toggle",
        mode_name
    );

    SDL_SetWindowTitle(application->display.window, title);
}

static void application_handle_event(
    Application *application,
    const SDL_Event *event
) {
    if (event->type == SDL_EVENT_QUIT) {
        application->running = 0;
        return;
    }

    if (event->type != SDL_EVENT_KEY_DOWN || event->key.repeat) {
        return;
    }

    switch (event->key.key) {
        case SDLK_ESCAPE:
            application->running = 0;
            break;

        case SDLK_W:
            application_set_rasterizer_mode(
                application,
                RASTERIZER_WIREFRAME
            );
            break;

        case SDLK_F:
            application_set_rasterizer_mode(
                application,
                RASTERIZER_FILLED
            );
            break;

        case SDLK_SPACE:
            application_set_rasterizer_mode(
                application,
                application->rasterizer_mode == RASTERIZER_WIREFRAME
                    ? RASTERIZER_FILLED
                    : RASTERIZER_WIREFRAME
            );
            break;

        // rotation
        case SDLK_LEFT:
            application->model = mat4_multiply(
                transform_make_rotation_y_matrix(-0.1f),
                application->model
            );
            break;

        case SDLK_RIGHT:
            application->model = mat4_multiply(
                transform_make_rotation_y_matrix(0.1f),
                application->model
            );
            break;

        case SDLK_UP:
            application->model = mat4_multiply(
                transform_make_rotation_x_matrix(-0.1f),
                application->model
            );
            break;

        case SDLK_DOWN:
            application->model = mat4_multiply(
                transform_make_rotation_x_matrix(0.1f),
                application->model
            );
            break;

        default:
            break;
    }
}

int application_create(
    Application *application,
    const char *model_path,
    int width,
    int height
) {
    *application = (Application){0};

    application->width = width;
    application->height = height;
    application->running = 1;
    application->clear_color = 0xFF000000;
    application->mesh_color = 0xFFFFFFFF;
    application->rasterizer_mode = RASTERIZER_WIREFRAME;

    if (!framebuffer_create(
            &application->framebuffer,
            width,
            height)) {
        return 0;
    }

    if (!display_create(
            &application->display,
            width,
            height)) {
        application_destroy(application);
        return 0;
    }

    if (!model_load(model_path, &application->mesh)) {
        application_destroy(application);
        return 0;
    }

    // fit model to bounding box
    application->model = transform_make_fit_matrix(
        &application->mesh
    );

    // move object
    application->view = transform_make_translation_matrix(
        0.0f,
        0.0f,
        -3.0f
    );

    // change projection matrix
    application->projection = transform_make_perspective_matrix(
        60.0f * 3.14159265359f / 180.0f,
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    if (!transform_mesh_to_screen(
            &application->mesh,
            &application->screen_mesh,
            application->model,
            application->view,
            application->projection,
            width,
            height)) {
        application_destroy(application);
        return 0;
    }

    application_update_window_title(application);
    return 1;
}

void application_run(Application *application) {
    while (application->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            application_handle_event(application, &event);
        }

        // update rotation, scale, move etc.
        transform_update_screen_vertices(
            &application->mesh,
            &application->screen_mesh,
            application->model,
            application->view,
            application->projection,
            application->width,
            application->height
        );

        framebuffer_clear(
            &application->framebuffer,
            application->clear_color
        );

        rasterizer_draw_mesh(
            &application->framebuffer,
            &application->screen_mesh,
            application->mesh_color,
            application->rasterizer_mode
        );

        display_present(
            &application->display,
            &application->framebuffer
        );
    }
}

void application_destroy(Application *application)
{
    screen_mesh_destroy(&application->screen_mesh);
    mesh_destroy(&application->mesh);

    if (application->display.window ||
        application->display.renderer ||
        application->display.texture) {
        display_destroy(&application->display);
    }

    if (application->framebuffer.pixels) {
        framebuffer_destroy(&application->framebuffer);
    }

    application->running = 0;
}

void application_set_rasterizer_mode(
    Application *application,
    RasterizerMode mode
) {
    application->rasterizer_mode = mode;
    application_update_window_title(application);
}
