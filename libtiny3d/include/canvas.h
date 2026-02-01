#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width;
    int height;
    float* pixels; // 1D grayscale pixel buffer: width * height
} canvas_t;

// Create a new blank canvas
canvas_t* canvas_create(int width, int height);

// Free canvas memory
void canvas_destroy(canvas_t* canvas);

// Save canvas as PGM image
void canvas_save_pgm(canvas_t* canvas, const char* filename);

// Set a pixel using bilinear filtering (anti-aliased)
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity);

// Draw an anti-aliased line with thickness using DDA
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness);

#endif