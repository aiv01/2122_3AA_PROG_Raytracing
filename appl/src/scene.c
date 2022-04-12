#include "scene.h"
#include "SDL.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "raytrace.h"

scene_t* scene_create(int screen_width, int screen_height, SDL_Renderer* r) {
   scene_t* scene = (scene_t*)malloc(sizeof(scene_t));
   scene->screen.width = screen_width;
   scene->screen.height = screen_height;
   scene->screen.aspect_ratio = (float)screen_width / (float)screen_height;
   scene->screen.render = r;

   scene->camera.fov = 60.f;
   float half_fov = scene->camera.fov / 2.f;
   float half_fov_rad = half_fov * M_PI / 180.f;
   scene->camera.fov_tan = tanf(half_fov_rad);
   scene->camera.position = (vector3_t){0.f, 0.f, 0.f};

   scene->sphere_count = 3;
   scene->spheres = (sphere_t*)malloc(sizeof(sphere_t) * scene->sphere_count);
   scene->spheres[0].position = (vector3_t){0.f, 0.f, -5.f};
   scene->spheres[0].radius = 1.f;
   scene->spheres[0].color = (color_t){1.f, 0.f, 0.f};

   scene->spheres[1].position = (vector3_t){-2.f, 0.f, -5.f};
   scene->spheres[1].radius = 1.f;
   scene->spheres[1].color = (color_t){0.f, 1.f, 0.f};

   scene->spheres[2].position = (vector3_t){2.f, 0.f, -5.f};
   scene->spheres[2].radius = 1.f;
   scene->spheres[2].color = (color_t){0.f, 0.f, 1.f};

   scene->light.direction = (vector3_t){0.f, -1.f, 0.f};
   scene->light.color = (color_t){1.f, 1.f, 1.f};

   scene->bg_color = (color_t){0.f, 0.f, 0.f};
   return scene;
}

void scene_update(scene_t* scene, float delta_time) {
   int width = scene->screen.width;
   int height = scene->screen.height;
   float aspect = scene->screen.aspect_ratio;
   float tan_fov = scene->camera.fov_tan;

   vector3_t ray_origin = scene->camera.position;

   for(int h=0; h < height;  ++h) {
      for(int w=0; w < width;  ++w) {
         float ndc_x = ((float)w + 0.5f)/(float)width;
         float ndc_y = ((float)h + 0.5f)/(float)height;

         float plane_x = 2.f * ndc_x - 1.f;
         float plane_y = 1.f - (2.f * ndc_y);

         plane_x *= aspect;

         plane_x *= tan_fov;
         plane_y *= tan_fov;

         ray_t ray;
         ray.origin = ray_origin;

         vector3_t plane_point = {plane_x, plane_y, -1.f};
         vector3_t ray_dir = vector3_sub(&plane_point, &ray_origin);
         ray.direction = vector3_norm(&ray_dir);

         color_t c = ray_trace(&ray, scene);

         SDL_SetRenderDrawColor(scene->screen.render, c.r * 255.f, c.g * 255.f, c.b * 255.f, 255);
         SDL_RenderDrawPoint(scene->screen.render, w, h);
      }   
   }

}

void scene_destroy(scene_t* scene) {
    free(scene);
}