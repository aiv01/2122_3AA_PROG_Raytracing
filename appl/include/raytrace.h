#pragma once
#include "vector3.h"
#include "color.h"
#include <stdbool.h>

typedef struct scene_t scene_t;
typedef struct sphere_t sphere_t;
typedef struct ray_t {
    vector3_t origin;
    vector3_t direction;
} ray_t;

typedef struct rayhit_t {
    float distance;
    sphere_t* object;
    vector3_t point;
    vector3_t normal;
} rayhit_t;

color_t ray_trace(ray_t* ray, scene_t* scene);
bool ray_cast(ray_t* ray, sphere_t* spheres, int sphere_count, rayhit_t* rayhit_out);