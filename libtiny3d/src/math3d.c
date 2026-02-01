#include <math.h>
#include "math3d.h"

// Convert spherical to Cartesian
vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.x = r * sinf(phi) * cosf(theta);
    v.y = r * sinf(phi) * sinf(theta);
    v.z = r * cosf(phi);
    return v;
}

// Normalize vector using fast inverse square root
vec3_t vec3_normalize_fast(vec3_t v) {
    float len_sq = v.x*v.x + v.y*v.y + v.z*v.z;
    float inv_len = 1.0f / sqrtf(len_sq);
    v.x *= inv_len;
    v.y *= inv_len;
    v.z *= inv_len;
    return v;
}

// Spherical linear interpolation
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    float dot = a.x*b.x + a.y*b.y + a.z*b.z;
    float theta = acosf(dot) * t;
    vec3_t rel = {
        b.x - a.x * dot,
        b.y - a.y * dot,
        b.z - a.z * dot
    };
    rel = vec3_normalize_fast(rel);
    vec3_t result = {
        a.x * cosf(theta) + rel.x * sinf(theta),
        a.y * cosf(theta) + rel.y * sinf(theta),
        a.z * cosf(theta) + rel.z * sinf(theta),
    };
    return result;
}

// Identity matrix
mat4_t mat4_identity() {
    mat4_t m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

// Translation matrix
mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

// Scaling matrix
mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = mat4_identity();
    m.m[0] = sx;
    m.m[5] = sy;
    m.m[10] = sz;
    return m;
}

// Rotation matrix from Euler angles (XYZ order)
mat4_t mat4_rotate_xyz(float rx, float ry, float rz) {
    float cx = cosf(rx), sx = sinf(rx);
    float cy = cosf(ry), sy = sinf(ry);
    float cz = cosf(rz), sz = sinf(rz);

    mat4_t m;
    m.m[0] = cy * cz;
    m.m[1] = sx * sy * cz - cx * sz;
    m.m[2] = cx * sy * cz + sx * sz;
    m.m[3] = 0.0f;

    m.m[4] = cy * sz;
    m.m[5] = sx * sy * sz + cx * cz;
    m.m[6] = cx * sy * sz - sx * cz;
    m.m[7] = 0.0f;

    m.m[8] = -sy;
    m.m[9] = sx * cy;
    m.m[10] = cx * cy;
    m.m[11] = 0.0f;

    m.m[12] = m.m[13] = m.m[14] = 0.0f;
    m.m[15] = 1.0f;

    return m;
}

// Perspective projection matrix
mat4_t mat4_frustum_asymmetric(float l, float r, float b, float t, float n, float f) {
    mat4_t m = {0};
    m.m[0] = 2 * n / (r - l);
    m.m[5] = 2 * n / (t - b);
    m.m[8] = (r + l) / (r - l);
    m.m[9] = (t + b) / (t - b);
    m.m[10] = -(f + n) / (f - n);
    m.m[11] = -1;
    m.m[14] = -(2 * f * n) / (f - n);
    return m;
}

// Apply matrix to vector (assume w = 1)
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v) {
    float x = m.m[0]*v.x + m.m[4]*v.y + m.m[8]*v.z + m.m[12];
    float y = m.m[1]*v.x + m.m[5]*v.y + m.m[9]*v.z + m.m[13];
    float z = m.m[2]*v.x + m.m[6]*v.y + m.m[10]*v.z + m.m[14];
    float w = m.m[3]*v.x + m.m[7]*v.y + m.m[11]*v.z + m.m[15];
    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }
    return (vec3_t){x, y, z};
}

// Multiply two matrices (column-major)
mat4_t mat4_mul(mat4_t a, mat4_t b) {
    mat4_t result;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            result.m[col*4 + row] =
                a.m[0*4 + row] * b.m[col*4 + 0] +
                a.m[1*4 + row] * b.m[col*4 + 1] +
                a.m[2*4 + row] * b.m[col*4 + 2] +
                a.m[3*4 + row] * b.m[col*4 + 3];
        }
    }
    return result;
}
