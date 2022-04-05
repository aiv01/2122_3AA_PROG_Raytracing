#ifndef SCENE_H
#define SCENE_H
#include "SDL.h"
#include "vector3.h"

typedef struct scene_t {
    struct {
        int width;
        int height;
        float aspect_ratio;
        SDL_Renderer* render;
    } screen;
    struct {
        float fov;
        float fov_tan;
        vector3_t position;
    } camera;
} scene_t;

scene_t* scene_create(int w, int h, SDL_Renderer*);
void scene_update(scene_t*, float delta_time);
void scene_destroy(scene_t*);

#endif //SCENE_H