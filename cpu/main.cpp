#include <iostream>

#include "include/common_header.h"
#include "include/hitable.h"
#include "include/sphere.h"
#include "include/camera.h"
#include "include/material.h"

int main() {

    // WORLD HITABLE OBJECT

    HITABLE_OBJECT_LIST world;

    auto ground_material = std::make_shared<LAMBERTIAN>(COLOR(0.5, 0.5, 0.5));
    world.add(std::make_shared<SPHERE>(POINT3(0,-1000,0), 1000, ground_material));
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            POINT3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

            if ((center - POINT3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<MATERIAL> SPHERE_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = COLOR::random_vec3() * COLOR::random_vec3();
                    SPHERE_material = std::make_shared<LAMBERTIAN>(albedo);
                    world.add(std::make_shared<SPHERE>(center, 0.2, SPHERE_material));
                } else if (choose_mat < 0.95) {
                    // METAL
                    auto albedo = COLOR::random_vec3(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    SPHERE_material = std::make_shared<METAL>(albedo, fuzz);
                    world.add(std::make_shared<SPHERE>(center, 0.2, SPHERE_material));
                } else {
                    // glass
                    SPHERE_material = std::make_shared<DIELECTRIC>(1.5);
                    world.add(std::make_shared<SPHERE>(center, 0.2, SPHERE_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<DIELECTRIC>(1.5);
    world.add(std::make_shared<SPHERE>(POINT3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<LAMBERTIAN>(COLOR(0.4, 0.2, 0.1));
    world.add(std::make_shared<SPHERE>(POINT3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<METAL>(COLOR(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<SPHERE>(POINT3(4, 1, 0), 1.0, material3));

    // CAMERA
    CAMERA cam;
    cam.set_image_width(1200);
    cam.set_samples_per_pixel(100);
    cam.set_look_at(POINT3(0, 0, 0));
    cam.set_look_from(POINT3(13, 2, 3));
    cam.set_vfov(20);
    cam.set_max_depth(50);
    cam.set_defocus_angle(0.6);
    cam.set_focus_distance(10);

    cam.render(world);

    return 0;
}