#include "vector3.h"
#include <math.h>

vector3_t vector3_sub(vector3_t* v1, vector3_t* v2) {
    vector3_t r;
    r.x = v1->x - v2->x;
    r.y = v1->y - v2->y;
    r.z = v1->z - v2->z;
    return r;
}

vector3_t vector3_norm(vector3_t* v) {
    float magn = vector3_magn(v);   
    vector3_t r;
    r.x = v->x / magn;
    r.y = v->y / magn;
    r.z = v->z / magn;
    return r;
}

float vector3_magn(vector3_t* v) { 
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}