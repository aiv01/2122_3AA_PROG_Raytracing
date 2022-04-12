#pragma once

typedef struct color_t {
    float r;
    float g;
    float b;
} color_t;

color_t color_mult_scal(color_t* c, float v);