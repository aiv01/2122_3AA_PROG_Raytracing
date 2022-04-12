#pragma once

typedef struct color_t {
    float r;
    float g;
    float b;
} color_t;

color_t color_mult_scal(color_t* c, float v);
color_t color_sum(color_t* c1, color_t* c2);
color_t color_clamp(color_t* c);