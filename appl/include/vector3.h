#pragma once

typedef struct vector3_t {
    float x;
    float y;
    float z;
} vector3_t;

vector3_t vector3_sub(vector3_t* v1, vector3_t* v2);
vector3_t vector3_norm(vector3_t* v);
float vector3_magn(vector3_t* v);