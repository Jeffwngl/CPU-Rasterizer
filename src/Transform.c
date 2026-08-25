#include "Transform.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

mat4 mat4_multiply(
    mat4 a,
    mat4 b
) {
    mat4 result = {0};

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[row][col] +=
                    a.m[row][i] *
                    b.m[i][col];
            }
        }
    }

    return result;
}


mat4 transform_make_rotation_x_matrix(
    float angle
) {
    mat4 matrix = mat4_identity();

    float c = cosf(angle);
    float s = sinf(angle);

    matrix.m[1][1] = c;
    matrix.m[1][2] = -s;

    matrix.m[2][1] = s;
    matrix.m[2][2] = c;

    return matrix;
}


mat4 transform_make_rotation_y_matrix(
    float angle
) {
    mat4 matrix = mat4_identity();

    float c = cosf(angle);
    float s = sinf(angle);

    matrix.m[0][0] = c;
    matrix.m[0][2] = s;

    matrix.m[2][0] = -s;
    matrix.m[2][2] = c;

    return matrix;
}


mat4 transform_make_translation_matrix(
    float x,
    float y,
    float z
) {
    mat4 matrix = mat4_identity();

    matrix.m[0][3] = x;
    matrix.m[1][3] = y;
    matrix.m[2][3] = z;

    return matrix;
}


mat4 transform_make_perspective_matrix(
    float fov,
    float aspect,
    float near,
    float far
) {
    mat4 matrix = {0};

    float f = 1.0f / tanf(fov * 0.5f);

    matrix.m[0][0] = f / aspect;
    matrix.m[1][1] = f;

    matrix.m[2][2] =
        (far + near) /
        (near - far);

    matrix.m[2][3] =
        (2.0f * far * near) /
        (near - far);

    matrix.m[3][2] = -1.0f;

    return matrix;
}

mat4 mat4_identity(void)
{
    mat4 matrix = {0};

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;

    return matrix;
}

// move object to fit screen better based on bounding box
mat4 transform_make_fit_matrix(
    const Mesh *mesh
) {
    mat4 matrix = mat4_identity();

    if (!mesh || !mesh->vertices || mesh->vertex_count == 0) {
        return matrix;
    }

    vec3 min = mesh->vertices[0].pos;
    vec3 max = mesh->vertices[0].pos;

    for (uint32_t i = 1; i < mesh->vertex_count; ++i) {
        vec3 pos = mesh->vertices[i].pos;

        if (pos.x < min.x) min.x = pos.x;
        if (pos.y < min.y) min.y = pos.y;
        if (pos.z < min.z) min.z = pos.z;

        if (pos.x > max.x) max.x = pos.x;
        if (pos.y > max.y) max.y = pos.y;
        if (pos.z > max.z) max.z = pos.z;
    }

    vec3 center = {
        (min.x + max.x) * 0.5f,
        (min.y + max.y) * 0.5f,
        (min.z + max.z) * 0.5f
    };

    float extent_x = max.x - min.x;
    float extent_y = max.y - min.y;
    float extent_z = max.z - min.z;

    float max_extent = extent_x;
    if (extent_y > max_extent) max_extent = extent_y;
    if (extent_z > max_extent) max_extent = extent_z;

    if (max_extent == 0.0f) {
        return matrix;
    }

    float scale = 1.6f / max_extent;

    matrix.m[0][0] = scale;
    matrix.m[1][1] = scale;
    matrix.m[2][2] = scale;

    matrix.m[0][3] = -center.x * scale;
    matrix.m[1][3] = -center.y * scale;
    matrix.m[2][3] = -center.z * scale;

    return matrix;
}

void transform_update_screen_vertices(
    const Mesh *mesh,
    ScreenMesh *screen_mesh,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
) {
    if (!mesh || !screen_mesh || !screen_mesh->vertices) {
        return;
    }

    for (uint32_t i = 0; i < mesh->vertex_count; ++i) {
        screen_mesh->vertices[i] = process_model_to_screen(
            mesh->vertices[i].pos,
            model,
            view,
            projection,
            width,
            height
        );
    }
}

int transform_mesh_to_screen(
    const Mesh *mesh,
    ScreenMesh *screen_mesh,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
) {
    *screen_mesh = (ScreenMesh){0};

    screen_mesh->vertex_count = mesh->vertex_count;
    screen_mesh->index_count = mesh->index_count;

    screen_mesh->vertices = malloc(
        mesh->vertex_count * sizeof(ScreenVertex)
    );

    if (!screen_mesh->vertices) {
        return 0;
    }

    screen_mesh->indices = malloc(
        mesh->index_count * sizeof(uint32_t)
    );

    if (!screen_mesh->indices) {
        free(screen_mesh->vertices);
        *screen_mesh = (ScreenMesh){0};
        return 0;
    }

    for (uint32_t i = 0; i < mesh->vertex_count; ++i) {
        screen_mesh->vertices[i] = process_model_to_screen(
            mesh->vertices[i].pos,
            model,
            view,
            projection,
            width,
            height
        );
    }

    /*
     * connectivity does not change during transformation,
     * so just copy the index buffer.
     */
    memcpy(
        screen_mesh->indices,
        mesh->indices,
        mesh->index_count * sizeof(uint32_t)
    );

    return 1;
}

void screen_mesh_destroy(ScreenMesh *mesh)
{
    free(mesh->vertices);
    free(mesh->indices);

    *mesh = (ScreenMesh){0};
}
