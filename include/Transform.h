#pragma once

#include <stdint.h>

#include "Model.h"
#include "Processing.h"

typedef struct {
    ScreenVertex *vertices;
    uint32_t vertex_count;

    uint32_t *indices;
    uint32_t index_count;
} ScreenMesh;

mat4 mat4_multiply(
    mat4 a,
    mat4 b
);

mat4 transform_make_rotation_x_matrix(
    float angle
);

mat4 transform_make_rotation_y_matrix(
    float angle
);

mat4 transform_make_translation_matrix(
    float x,
    float y,
    float z
);

mat4 transform_make_perspective_matrix(
    float fov,
    float aspect,
    float near,
    float far
);

mat4 mat4_identity(void);

// fit object to screen in case away from origin
mat4 transform_make_fit_matrix(
    const Mesh *mesh
);

// update vertices for rotation or transformation
void transform_update_screen_vertices(
    const Mesh *mesh,
    ScreenMesh *screen_mesh,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
);

int transform_mesh_to_screen(
    const Mesh *mesh,
    ScreenMesh *screen_mesh,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
);

void screen_mesh_destroy(
    ScreenMesh *mesh
);
