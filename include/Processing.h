#pragma once

#include "Framebuffer.h"
#include "Model.h"

typedef struct {
     float x;
     float y;
     float z;
} ScreenVertex;

ScreenVertex process_model_to_screen(
    vec3 pos,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
);

vec4 process_model_to_world(
    vec3 pos,
    mat4 model
);

vec4 process_world_to_view(
    vec4 pos,
    mat4 view
);

vec4 process_view_to_clip(
    vec4 pos,
    mat4 projection
);

vec3 process_clip_to_ndc(
    vec4 clip
);

ScreenVertex process_ndc_to_screen(
    vec3 ndc,
    int width,
    int height
);
