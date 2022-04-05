#include "raytrace.h"
#include "scene.h"
#include <math.h>

bool ray_cast(ray_t* ray, sphere_t* sphere, rayhit_t* rayhit_out) {
    vector3_t L = vector3_sub(&sphere->position, &ray->origin);
    float Tca = vector3_dot(&L, &ray->direction);
    if (Tca < 0.f) return false;
    float d2 = vector3_magn2(&L) - Tca*Tca;
    float radius2 = sphere->radius * sphere->radius;
    if (d2 > radius2) return false;
    float Thc = sqrtf(radius2 - d2);

    float t0 = Tca - Thc;
    float t1 = Tca + Thc;
    rayhit_out->distance = fminf(t0, t1);
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
    float lambert = vector3_dot(&shadow_ray.direction, &hit.normal);
    if (lambert < 0) lambert = 0.f;
    //else if (lambert > 1.f) lambert = 1.f;

    color_t c = hit.object->color;
    c.r = c.r * lambert;
    c.g = c.g * lambert;
    c.b = c.b * lambert;
    return c;
}