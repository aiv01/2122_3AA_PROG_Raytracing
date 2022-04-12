#include "raytrace.h"
#include "scene.h"
#include <math.h>
#include <float.h>


bool sphere_intersect(sphere_t* sphere, ray_t* ray, rayhit_t* rayhit_out) {
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


bool ray_cast(ray_t* ray, sphere_t* spheres, int sphere_count, rayhit_t* rayhit_out) {
    rayhit_t best_hit;
    best_hit.distance = FLT_MAX;
    bool has_best_hit = false;
    for(int i=0; i < sphere_count; ++i) {
        sphere_t* each = spheres + i;
        rayhit_t hit;
        bool has_hit = sphere_intersect(each, ray, &hit);
        if (has_hit && hit.distance < best_hit.distance) {
            best_hit = hit;
            has_best_hit = true;
        }
    }
    *rayhit_out = best_hit;
    return has_best_hit;
}

color_t ray_trace(ray_t* ray, scene_t* scene) {
    //Primary Ray
    rayhit_t hit;
    bool has_hit = ray_cast(ray, scene->spheres, scene->sphere_count, &hit);
    if (!has_hit) return scene->bg_color;

    //Shadow Ray
    ray_t shadow_ray;
    shadow_ray.origin = hit.point; 

    vector3_t inverted_light_dir = vector3_mult_scal(&scene->light.direction, -1.f);
    shadow_ray.direction = inverted_light_dir;

    rayhit_t shadow_hit;
    bool shadow_has_hit = ray_cast(&shadow_ray, scene->spheres, scene->sphere_count, &shadow_hit);
    if (shadow_has_hit) return scene->bg_color;

    
    //Ambient
    //TODO

    //Diffuse
    float lambert = fmaxf(0.f, vector3_dot(&shadow_ray.direction, &hit.normal));
    color_t diffuse = color_mult_scal(&hit.object->color, lambert);
    
    //Specular
    vector3_t* L = &shadow_ray.direction;
    vector3_t V = vector3_mult_scal(&hit.point, -1.f);
    vector3_t H = vector3_sum(L, &V);
    H = vector3_norm(&H);
    float specular_strenght = fmaxf(0.f, vector3_dot(&hit.normal, &H));
    float specular_intesity = powf(specular_strenght, 100.f);
    color_t specular = color_mult_scal(&scene->light.color, specular_intesity);

    color_t final = {0.f, 0.f, 0.f};
    final = color_sum(&final, &diffuse);
    final = color_sum(&final, &specular);
    final = color_clamp(&final);
    return final;
}