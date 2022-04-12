#include "color.h"
#include <math.h>

color_t color_mult_scal(color_t* c, float v) {
    color_t res;
    res.r = c->r * v;
    res.g = c->g * v;
    res.b = c->b * v;
    return res;
}

color_t color_sum(color_t* c1, color_t* c2) {
    color_t res;
    res.r = c1->r + c2->r;
    res.g = c1->g + c2->g;
    res.b = c1->b + c2->b;
    return res;
}

color_t color_mult(color_t* c1, color_t* c2) {
    color_t res;
    res.r = c1->r * c2->r;
    res.g = c1->g * c2->g;
    res.b = c1->b * c2->b;
    return res;
}


color_t color_clamp(color_t* c) {
    color_t res;
    res.r = fminf(1.f, c->r);
    res.g = fminf(1.f, c->g);
    res.b = fminf(1.f, c->b);
    return res;
}

color_t color_black() {
    color_t c = {0.f, 0.f, 0.f};
    return c;
}

color_t color_red() {
    color_t c = {1.f, 0.f, 0.f};
    return c;
}

color_t color_white() {
    color_t c = {1.f, 1.f, 1.f};
    return c;
}