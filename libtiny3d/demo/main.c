#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"
#include "animation.h"

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

    // Lighting setup
    light_t lights[2] = {
        {{ 0.0f, 0.0f, -1.0f }, 0.8f},
        {{ 1.0f, 1.0f, -1.0f }, 0.5f}
    };

    // Bezier paths for two animated objects
    vec3_t path1[4] = {{-1, 0, 0}, {-0.5, 1, -1}, {0.5, -1, -1}, {1, 0, 0}};
    vec3_t path2[4] = {{1, 0, 0}, {1, 1, 1}, {-1, -1, 1}, {-1, 0, 0}};

    for (int frame = 0; frame < FRAMES; frame++) {
        canvas_t* canvas = canvas_create(WIDTH, HEIGHT);
        if (!canvas) {
            fprintf(stderr, "Failed to create canvas\n");
            return 1;
        }

        float angle = (2 * M_PI * frame) / FRAMES;
        float t = (float)frame / FRAMES;

        // Compute positions along Bezier paths
        vec3_t offset1 = bezier(path1[0], path1[1], path1[2], path1[3], t);
        vec3_t offset2 = bezier(path2[0], path2[1], path2[2], path2[3], t);

        // Transformation for object 1
        mat4_t model1 = mat4_mul(
            mat4_translate(offset1.x, offset1.y, -3 + offset1.z),
            mat4_rotate_xyz(angle, angle * 0.8f, 0)
        );

        // Transformation for object 2
        mat4_t model2 = mat4_mul(
            mat4_translate(offset2.x, offset2.y, -3 + offset2.z),
            mat4_rotate_xyz(-angle * 0.5f, angle, 0)
        );

        mat4_t proj = mat4_frustum_asymmetric(-1, 1, -1, 1, 1.0f, 10.0f);
        mat4_t mvp1 = mat4_mul(proj, model1);
        mat4_t mvp2 = mat4_mul(proj, model2);

        // Render both moving and rotating objects
        render_wireframe(canvas, vertices, vertex_count, edges, edge_count, mvp1, lights, 2);
        render_wireframe(canvas, vertices, vertex_count, edges, edge_count, mvp2, lights, 2);

        char filename[64];
        sprintf(filename, "frame_%03d.pgm", frame);
        canvas_save_pgm(canvas, filename);
        canvas_destroy(canvas);
    }

    printf("Frames saved.\n");
    return 0;
}
