#include "lighting.h"
#include <math.h>

float compute_lighting(vec3_t edge_dir, light_t* lights, int light_count) {
    float total = 0.0f;
    for (int i = 0; i < light_count; i++) {
        float d = edge_dir.x * lights[i].direction.x +
                  edge_dir.y * lights[i].direction.y +
                  edge_dir.z * lights[i].direction.z;
        if (d > 0.0f) {
            total += d * lights[i].intensity;
        }
    }
    if (total > 1.0f) total = 1.0f;
    return total;
}
