#pragma once
#include "vector3.h"

typedef struct ray_t {
    vector3_t origin;
    vector3_t direction;
} ray_t;

typedef struct color_t {
    float r;
    float g;
    float b;
} color_t;

typedef struct scene_t scene_t;

color_t ray_trace(ray_t* ray, scene_t* scene);