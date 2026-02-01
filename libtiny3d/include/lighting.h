#ifndef LIGHTING_H
#define LIGHTING_H

#include "math3d.h"

#define MAX_LIGHTS 4

typedef struct {
    vec3_t direction;
    float intensity;
} light_t;

float compute_lighting(vec3_t edge_dir, light_t* lights, int light_count);

#endif
