// test_math.c — Task 2: 3D Math Foundation Test
#include <stdio.h>
#include <math.h>
#include "math3d.h"
#include "canvas.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// A cube with 8 vertices
vec3_t cube_vertices[8] = {
    {-1, -1, -1},
    { 1, -1, -1},
    { 1,  1, -1},
    {-1,  1, -1},
    {-1, -1,  1},
    { 1, -1,  1},
    { 1,  1,  1},
    {-1,  1,  1}
};

// 12 edges of the cube
int cube_edges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};

void project_and_draw_cube(canvas_t* canvas, mat4_t mvp) {
    for (int i = 0; i < 12; i++) {
        vec3_t a = cube_vertices[cube_edges[i][0]];
        vec3_t b = cube_vertices[cube_edges[i][1]];

        vec3_t pa = mat4_mul_vec3(mvp, a);
        vec3_t pb = mat4_mul_vec3(mvp, b);

        float x0 = (pa.x + 1.0f) * 0.5f * canvas->width;
        float y0 = (1.0f - pa.y) * 0.5f * canvas->height;
        float x1 = (pb.x + 1.0f) * 0.5f * canvas->width;
        float y1 = (1.0f - pb.y) * 0.5f * canvas->height;

        draw_line_f(canvas, x0, y0, x1, y1, 1.5f);
    }
}

int main() {
    int width = 800, height = 800;
    canvas_t* canvas = canvas_create(width, height);

    // Create transform matrices
    mat4_t proj = mat4_frustum_asymmetric(-1, 1, -1, 1, 1.5, 10);
    mat4_t view = mat4_translate(0, 0, -5);  // camera backward
    mat4_t model = mat4_rotate_xyz(0.6f, 0.7f, 0.2f);  // some rotation

    // MVP = proj * view * model
    mat4_t mvp = mat4_mul(proj, mat4_mul(view, model));

    project_and_draw_cube(canvas, mvp);

    canvas_save_pgm(canvas, "task2_cube.pgm");
    canvas_destroy(canvas);
    printf("Saved cube projection to task2_cube.pgm\n");
    return 0;
}
