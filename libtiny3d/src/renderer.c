#include <math.h>
#include <stdlib.h>
#include "renderer.h"
#include "canvas.h"
#include "math3d.h"
#include "lighting.h"

int clip_to_circle(canvas_t* canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float r = canvas->width / 2.2f;
    float dx = x - cx;
    float dy = y - cy;
    return dx * dx + dy * dy <= r * r;
}

vec3_t project_vertex(vec3_t v, mat4_t mvp, int width, int height) {
    vec3_t p = mat4_mul_vec3(mvp, v);
    p.x = (p.x + 1.0f) * 0.5f * width;
    p.y = (1.0f - p.y) * 0.5f * height;
    return p;
}

void render_wireframe(canvas_t* canvas, vec3_t* vertices, int vertex_count, int (*edges)[2], int edge_count, mat4_t mvp, light_t* lights, int light_count) {
    vec3_t* projected = malloc(sizeof(vec3_t) * vertex_count);
    vec3_t* transformed = malloc(sizeof(vec3_t) * vertex_count);
    for (int i = 0; i < vertex_count; i++) {
        transformed[i] = mat4_mul_vec3(mvp, vertices[i]);
        projected[i] = project_vertex(vertices[i], mvp, canvas->width, canvas->height);
    }

    for (int i = 0; i < edge_count; i++) {
        int a = edges[i][0];
        int b = edges[i][1];
        float x0 = projected[a].x, y0 = projected[a].y;
        float x1 = projected[b].x, y1 = projected[b].y;

        if (clip_to_circle(canvas, x0, y0) && clip_to_circle(canvas, x1, y1)) {
            vec3_t world_a = vertices[a];
            vec3_t world_b = vertices[b];
            vec3_t edge_dir = {
                world_b.x - world_a.x,
                world_b.y - world_a.y,
                world_b.z - world_a.z
            };
            edge_dir = vec3_normalize_fast(edge_dir);

            float intensity = compute_lighting(edge_dir, lights, light_count);
            draw_line_f(canvas, x0, y0, x1, y1, 1.5f * intensity);
        }
    }

    free(projected);
    free(transformed);
}
