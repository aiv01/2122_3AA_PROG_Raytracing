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


bool ray_cast(ray_t* ray, sphere_t* spheres, int sphere_count, rayhit_t* rayhit_out, ray_cast_strategy strategy) {
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
            if (strategy == RAYCAST_FIRST) break;
        }
    }
    *rayhit_out = best_hit;
    return has_best_hit;
}

color_t ray_trace(ray_t* ray, scene_t* scene, int current_depth) {
    if (current_depth > 13) return scene->bg_color;
    //Primary Ray
    rayhit_t hit;
    bool has_hit = ray_cast(ray, scene->spheres, scene->sphere_count, &hit, RAYCAST_BEST);
    if (!has_hit) return scene->bg_color;

    //Shadow Ray
    float bias = 1e-4;
    vector3_t biased_normal = vector3_mult_scal(&hit.normal, bias);
    vector3_t biased_hit_point = vector3_sum(&hit.point, &biased_normal);

    ray_t shadow_ray;
    shadow_ray.origin = biased_hit_point; 

    vector3_t inverted_light_dir = vector3_mult_scal(&scene->light.direction, -1.f);
    shadow_ray.direction = inverted_light_dir;

    rayhit_t shadow_hit;
    bool shadow_has_hit = ray_cast(&shadow_ray, scene->spheres, scene->sphere_count, &shadow_hit, RAYCAST_FIRST);
    if (shadow_has_hit) return scene->bg_color;

    
    //Ambient
    //TODO

    //Diffuse
    float lambert = fmaxf(0.f, vector3_dot(&shadow_ray.direction, &hit.normal));
    lambert *= (1.f - hit.object->material.reflect_factor);
    color_t diffuse = color_mult_scal(&hit.object->material.albedo, lambert);
    
    //Specular
    vector3_t* L = &shadow_ray.direction;
    vector3_t V = vector3_mult_scal(&hit.point, -1.f);
    vector3_t H = vector3_sum(L, &V);
    H = vector3_norm(&H);
    float specular_strenght = fmaxf(0.f, vector3_dot(&hit.normal, &H));
    float specular_intesity = powf(specular_strenght, hit.object->material.specular_shiness_factor);
    color_t specular = color_mult_scal(&scene->light.color, specular_intesity);
    specular = color_mult(&specular, &hit.object->material.specular_color);

    color_t blinn_phong = {0.f, 0.f, 0.f};
    blinn_phong = color_sum(&blinn_phong, &diffuse);
    blinn_phong = color_sum(&blinn_phong, &specular);
    
    vector3_t refl_vect = vector3_refl(&ray->direction, &hit.normal);
    //? normalizzare
    ray_t refl_ray;
    refl_ray.origin = biased_hit_point;
    refl_ray.direction = refl_vect;

    color_t refl_color = ray_trace(&refl_ray, scene, current_depth + 1);
    refl_color = color_mult_scal(&refl_color, hit.object->material.reflect_factor);
    
    color_t final = color_black();
    final = color_sum(&final, &blinn_phong);
    final = color_sum(&final, &refl_color);
    final = color_clamp(&final);
    return final;
}