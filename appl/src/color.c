#include "color.h"

color_t color_mult_scal(color_t* c, float v) {
    color_t res;
    res.r = c->r * v;
    res.g = c->g * v;
    res.b = c->b * v;
    return res;
}