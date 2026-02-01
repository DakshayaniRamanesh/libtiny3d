#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH 512
#define HEIGHT 512
#define FRAMES 60

#define MAX_VERTICES 1024
#define MAX_EDGES 2048

// Load .obj with 'v' and 'f' lines, convert faces to edges
int load_obj(const char* filename, vec3_t* vertices, int* vertex_count, int edges[][2], int* edge_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return 0;
    }

    char line[512];
    int v_count = 0, e_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3_t v;
            if (sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z) == 3) {
                if (v_count < MAX_VERTICES) {
                    vertices[v_count++] = v;
                }
            }
        } else if (line[0] == 'f' && line[1] == ' ') {
            int indices[10];
            int count = 0;

            char* token = strtok(line + 2, " \n");
            while (token && count < 10) {
                int i;
                if (sscanf(token, "%d", &i) == 1) {
                    indices[count++] = i - 1;  // OBJ is 1-based
                }
                token = strtok(NULL, " \n");
            }

            for (int i = 0; i < count; i++) {
                int a = indices[i];
                int b = indices[(i + 1) % count];
                if (e_count < MAX_EDGES) {
                    edges[e_count][0] = a;
                    edges[e_count][1] = b;
                    e_count++;
                }
            }
        }
    }

    fclose(file);
    *vertex_count = v_count;
    *edge_count = e_count;
    return 1;
}

int main() {
    vec3_t vertices[MAX_VERTICES];
    int vertex_count = 0;
    int edges[MAX_EDGES][2];
    int edge_count = 0;

    if (!load_obj("soccer.obj", vertices, &vertex_count, edges, &edge_count)) {
        fprintf(stderr, "Failed to load soccer.obj\n");
        return 1;
    }

    for (int frame = 0; frame < FRAMES; frame++) {
        canvas_t* canvas = canvas_create(WIDTH, HEIGHT);
        if (!canvas) {
            fprintf(stderr, "Failed to create canvas\n");
            return 1;
        }

        float angle = (2 * M_PI * frame) / FRAMES;

        mat4_t model = mat4_mul(
            mat4_rotate_xyz(angle, angle * 0.8f, 0),
            mat4_scale(1.2f, 1.2f, 1.2f)
        );
        model = mat4_mul(mat4_translate(0, 0, -3.0f), model);

        mat4_t proj = mat4_frustum_asymmetric(-1, 1, -1, 1, 1.0f, 10.0f);
        mat4_t mvp = mat4_mul(proj, model);

        render_wireframe(canvas, vertices, vertex_count, edges, edge_count, mvp);

        char filename[64];
        sprintf(filename, "frame_%03d.pgm", frame);
        canvas_save_pgm(canvas, filename);
        canvas_destroy(canvas);
    }

    printf("Frames saved.\n");
    return 0;
}