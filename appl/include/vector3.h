#pragma once

typedef struct vector3_t {
    float x;
    float y;
    float z;
} vector3_t;

vector3_t vector3_sub(vector3_t* v1, vector3_t* v2);
vector3_t vector3_norm(vector3_t* v);
float vector3_magn(vector3_t* v);
float vector3_magn2(vector3_t* v);
float vector3_dot(vector3_t* v1, vector3_t* v2);
vector3_t vector3_mult_scal(vector3_t* v, float scalar);
vector3_t vector3_sum(vector3_t* v1, vector3_t* v2);
vector3_t vector3_refl(vector3_t* v, vector3_t* n);