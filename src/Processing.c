#include "Processing.h"

static vec4 mat4_mult(
    mat4 m,
    vec4 v
) {
    vec4 result;

    result.x =
        m.m[0][0] * v.x +
        m.m[0][1] * v.y +
        m.m[0][2] * v.z +
        m.m[0][3] * v.w;

    result.y =
        m.m[1][0] * v.x +
        m.m[1][1] * v.y +
        m.m[1][2] * v.z +
        m.m[1][3] * v.w;

    result.z =
        m.m[2][0] * v.x +
        m.m[2][1] * v.y +
        m.m[2][2] * v.z +
        m.m[2][3] * v.w;

    result.w =
        m.m[3][0] * v.x +
        m.m[3][1] * v.y +
        m.m[3][2] * v.z +
        m.m[3][3] * v.w;

    return result;       
}

ScreenVertex process_model_to_screen(
    vec3 pos,
    mat4 model,
    mat4 view,
    mat4 projection,
    int width,
    int height
) {
    vec4 worldPos = process_model_to_world(
        pos,
        model
    );

    vec4 viewPos = process_world_to_view(
        worldPos,
        view
    );

    vec4 clipPos = process_view_to_clip(
        viewPos,
        projection
    );

    vec3 ndc = process_clip_to_ndc(
        clipPos
    );

    ScreenVertex screenVertex = process_ndc_to_screen(
        ndc,
        width,
        height
    );

    return screenVertex;
}

vec4 process_model_to_world(
    vec3 pos,
    mat4 model
) {
    vec4 local = {
        pos.x,
        pos.y,
        pos.z,
        1.0f
    };

    vec4 worldPos = mat4_mult(
        model,
        local
    );

    return worldPos;
}

vec4 process_world_to_view(
    vec4 pos,
    mat4 view
) {
    vec4 viewPos = mat4_mult(
        view,
        pos
    );

    return viewPos;
}

vec4 process_view_to_clip(
    vec4 pos,
    mat4 projection
) {
    vec4 clipPos = mat4_mult(
        projection,
        pos
    );

    return clipPos;
}


vec3 process_clip_to_ndc(
    vec4 pos
) {
    if (pos.w == 0) {
        vec3 ndcPos = {
            pos.x,
            pos.y,
            pos.z
        };
        
        return ndcPos;
    }

    vec3 ndcPos = {
        pos.x / pos.w,
        pos.y / pos.w,
        pos.z / pos.w
    };

    return ndcPos;
}

ScreenVertex process_ndc_to_screen(
    vec3 ndc,
    int width,
    int height
) {
    ScreenVertex screenPos = {
        (ndc.x + 1.0f) * 0.5f * width,
        (1.0f - ndc.y) * 0.5f * height,
        ndc.z
    };

    return screenPos;
}
