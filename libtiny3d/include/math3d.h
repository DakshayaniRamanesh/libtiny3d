#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    union {
        struct { float x, y, z; };
        struct { float r, theta, phi; }; // spherical
    };
} vec3_t;

typedef struct {
    float m[16]; // 4x4 matrix, column-major
} mat4_t;

// Vector operations
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);

// Matrix operations
mat4_t mat4_identity();
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_xyz(float rx, float ry, float rz);
mat4_t mat4_frustum_asymmetric(float l, float r, float b, float t, float n, float f);
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v);
mat4_t mat4_mul(mat4_t a, mat4_t b);

#endif