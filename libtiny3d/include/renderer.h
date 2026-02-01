#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"
#include "lighting.h"
#include "canvas.h"

int clip_to_circle(canvas_t* canvas, float x, float y);

vec3_t project_vertex(vec3_t v, mat4_t mvp, int width, int height);

// Now supports lighting
void render_wireframe(
    canvas_t* canvas,
    vec3_t* vertices,
    int vertex_count,
    int (*edges)[2],
    int edge_count,
    mat4_t mvp,
    light_t* lights,
    int light_count
);

#endif // RENDERER_H
