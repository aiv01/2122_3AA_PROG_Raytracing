#include "raytrace.h"
#include "scene.h"
#include <math.h>

bool ray_cast(ray_t* ray, sphere_t* sphere, rayhit_t* rayhit_out) {
    vector3_t L = vector3_sub(&sphere->position, &ray->origin);
    float Tca = vector3_dot(&L, &ray->direction);
    if (Tca < 0.f) return false; // Questo garantisce che Tca e' sempre positivo, per cui t0 sara' sempre il punto piu vicino
    float d2 = vector3_magn2(&L) - Tca*Tca;
    float radius2 = sphere->radius * sphere->radius;
    if (d2 > radius2) return false;
    float Thc = sqrtf(radius2 - d2);

     
    float t0 = Tca - Thc;       //Nel caso in cui il Ray origin e' dentro la sfera allora t0 viene negativo poiche' Tca < Thc
    float t1 = Tca + Thc;
    //if (t0 < t1 ) rayhit_out->distance = t0; 
    //else rayhit_out->distance = t1; 

    if (t1 < t0) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    if (t0 < 0) { 
        t0 = t1;
        if (t0 < 0) return false;
    }
    rayhit_out->distance = t0; 
    rayhit_out->object = sphere;

    vector3_t unnorm_dir = vector3_mult_scal(&ray->direction, rayhit_out->distance);
    rayhit_out->point = vector3_sum(&ray->origin, &unnorm_dir);

    vector3_t unnorm_normal = vector3_sub(&rayhit_out->point, &sphere->position);
    rayhit_out->normal = vector3_norm(&unnorm_normal);
    return true;
}

color_t ray_trace(ray_t* ray, scene_t* scene) {
    //Primary Ray
    rayhit_t hit;
    bool has_hit = ray_cast(ray, &scene->sphere, &hit);
    if (!has_hit) return scene->bg_color;

    //Shadow Ray
    ray_t shadow_ray;
    shadow_ray.origin = hit.point; 

    vector3_t inverted_light_dir = vector3_mult_scal(&scene->light.direction, -1.f);
    shadow_ray.direction = inverted_light_dir;

    rayhit_t shadow_hit;
    bool shadow_has_hit = ray_cast(&shadow_ray, &scene->sphere, &shadow_hit);
    if (shadow_has_hit) return scene->bg_color;

    //Shading Diffuse
    float lambert = fmaxf(0.f, vector3_dot(&shadow_ray.direction, &hit.normal));
    
    color_t c = color_mult_scal(&hit.object->color, lambert);
    return c;
}