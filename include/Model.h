#pragma once

#include <stdint.h>

#include "MathTypes.h"

typedef struct {
    vec3 pos;
} Vertex;

typedef struct {
    Vertex *vertices;
    uint32_t vertex_count;
    uint32_t vertex_capacity;

    uint32_t *indices;
    uint32_t index_count;
    uint32_t index_capacity;
} Mesh;

int model_load(
    const char* path,
    Mesh* mesh 
);

void mesh_destroy(
    Mesh* mesh
);
