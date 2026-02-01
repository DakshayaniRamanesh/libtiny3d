// main.c – Task 1: Line Drawing Demo
#include <stdio.h>
#include <math.h>
#include "canvas.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    int width = 800;
    int height = 800;
    canvas_t* canvas = canvas_create(width, height);

    float cx = width / 2.0f;
    float cy = height / 2.0f;
    float radius = 350.0f;

    // Draw lines like clock hands (every 15°)
    for (int angle_deg = 0; angle_deg < 360; angle_deg += 15) {
        float angle_rad = angle_deg * M_PI / 180.0f;
        float x = cx + radius * cosf(angle_rad);
        float y = cy + radius * sinf(angle_rad);
        draw_line_f(canvas, cx, cy, x, y, 2.0f);  // thickness = 2.0
    }

    canvas_save_pgm(canvas, "task1_output.pgm");
    printf("Saved output to task1_output.pgm\n");

    canvas_destroy(canvas);
    return 0;
}
