#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"

// Create canvas and initialize to 0.0 (black)
canvas_t* canvas_create(int width, int height) {
    canvas_t* c = malloc(sizeof(canvas_t));
    c->width = width;
    c->height = height;
    c->pixels = calloc(width * height, sizeof(float));
    return c;
}

void canvas_destroy(canvas_t* c) {
    if (c) {
        free(c->pixels);
        free(c);
    }
}

void canvas_save_pgm(canvas_t* c, const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "P2\n%d %d\n255\n", c->width, c->height);
    for (int y = 0; y < c->height; y++) {
        for (int x = 0; x < c->width; x++) {
            float intensity = c->pixels[y * c->width + x];
            if (intensity < 0.0f) intensity = 0.0f;
            if (intensity > 1.0f) intensity = 1.0f;
            int pixel = (int)(intensity * 255);
            fprintf(f, "%d ", pixel);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

// Set pixel using bilinear filtering (splits brightness to 4 nearby pixels)
void set_pixel_f(canvas_t* c, float x, float y, float intensity) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 1; j++) {
            int xi = x0 + i;
            int yj = y0 + j;
            if (xi >= 0 && xi < c->width && yj >= 0 && yj < c->height) {
                float weight = (1.0f - fabsf(dx - i)) * (1.0f - fabsf(dy - j));
                int index = yj * c->width + xi;
                c->pixels[index] += intensity * weight;
                if (c->pixels[index] > 1.0f)
                    c->pixels[index] = 1.0f;
            }
        }
    }
}

// Draw anti-aliased line with thickness using DDA and circle splatting
void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = sqrtf(dx * dx + dy * dy);
    int steps = (int)(length * 2); // finer sampling
    float radius = thickness / 2.0f;

    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float x = x0 + t * dx;
        float y = y0 + t * dy;

        // Draw a soft circular dot around (x, y)
        int xmin = (int)(x - radius - 1);
        int xmax = (int)(x + radius + 1);
        int ymin = (int)(y - radius - 1);
        int ymax = (int)(y + radius + 1);

        for (int xi = xmin; xi <= xmax; xi++) {
            for (int yi = ymin; yi <= ymax; yi++) {
                float dist2 = (x - xi) * (x - xi) + (y - yi) * (y - yi);
                if (dist2 <= radius * radius) {
                    float falloff = 1.0f - (dist2 / (radius * radius));
                    if (falloff > 0.0f)
                        set_pixel_f(c, xi, yi, falloff);
                }
            }
        }
    }
}