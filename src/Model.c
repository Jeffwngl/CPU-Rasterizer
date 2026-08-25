#include "Model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// helper to manually resize array similar to cpp vector resizing

static int push_vertex(Mesh *mesh, Vertex vertex)
{
    if (mesh->vertex_count >= mesh->vertex_capacity) {
        uint32_t new_capacity =
            mesh->vertex_capacity == 0
            ? 16
            : mesh->vertex_capacity * 2;

        Vertex *new_vertices = realloc(
            mesh->vertices,
            new_capacity * sizeof(Vertex)
        );

        if (!new_vertices) {
            return 0;
        }

        mesh->vertices = new_vertices;
        mesh->vertex_capacity = new_capacity;
    }

    mesh->vertices[mesh->vertex_count++] = vertex;
    return 1;
}

static int push_index(Mesh *mesh, uint32_t index)
{
    if (mesh->index_count >= mesh->index_capacity) {
        uint32_t new_capacity =
            mesh->index_capacity == 0
            ? 16
            : mesh->index_capacity * 2;

        uint32_t *new_indices = realloc(
            mesh->indices,
            new_capacity * sizeof(uint32_t)
        );

        if (!new_indices) {
            return 0;
        }

        mesh->indices = new_indices;
        mesh->index_capacity = new_capacity;
    }

    mesh->indices[mesh->index_count++] = index;
    return 1;
}

// used for obj file layout for triangles starting with f e.g. 1\1\1
static int parse_face_index(
    const char *token,
    unsigned int *index
) {
    if (sscanf(token, "%u", index) != 1) {
        return 0;
    }

    if (*index == 0) {
        return 0;
    }

    return 1;
}


static int load_obj(
    FILE* file,
    Mesh* mesh
) {
    char line[1024];

    while (fgets(line, sizeof(line), file)) {

        /*
         * vertex:
         *
         * v 1.0 2.0 3.0
         */
        if (
            line[0] == 'v' &&
            line[1] == ' '
        ) {
            Vertex vertex;

            int result = sscanf(
                line,
                "v %f %f %f",
                &vertex.pos.x,
                &vertex.pos.y,
                &vertex.pos.z
            );

            if (result != 3) {
                fprintf(stderr, "Invalid vertex: %s", line);
                mesh_destroy(mesh);
                return 0;
            }

            if (!push_vertex(mesh, vertex)) {
                mesh_destroy(mesh);
                return 0;
            }
        }

        /*
         * triangle:
         *
         * f 1 2 3 or f1\1\1
         */
        else if (
            line[0] == 'f' &&
            line[1] == ' '
        ) {
            char a_token[64];
            char b_token[64];
            char c_token[64];

            int result = sscanf(
                line,
                "f %63s %63s %63s",
                a_token,
                b_token,
                c_token
            );

            if (result != 3) {
                fprintf(stderr, "Invalid face: %s", line);
                mesh_destroy(mesh);
                return 0;
            }

            unsigned int a;
            unsigned int b;
            unsigned int c;

            if (
                !parse_face_index(a_token, &a) ||
                !parse_face_index(b_token, &b) ||
                !parse_face_index(c_token, &c)
            ) {
                fprintf(stderr, "Invalid OBJ index\n");
                mesh_destroy(mesh);
                return 0;
            }

            /*
            * OBJ indices start from 1
            * C arrays start from 0
            */
            if (
                !push_index(mesh, a - 1) ||
                !push_index(mesh, b - 1) ||
                !push_index(mesh, c - 1)
            ) {
                mesh_destroy(mesh);
                return 0;
            }
        }
    }

    return 1;
}



int model_load(
    const char* path,
    Mesh* mesh
) {

    const char *extension = strrchr(path, '.');

    FILE *file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "Failed to open path: %s\n", path);
        return 0;
    }

    *mesh = (Mesh){0};

    if (extension && strcmp(extension, ".obj") == 0) {
        if (!load_obj(file, mesh)) {
            fclose(file);
            return 0;
        }
        printf("Loaded .obj file\n");
    }
    else {
        fprintf(stderr, "Not a valid file format.\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    return 1;
}

void mesh_destroy(
    Mesh* mesh
) {
    free(mesh->vertices);
    free(mesh->indices);

    *mesh = (Mesh){0};
}
