#include <iostream>

#include "include/common_header.h"
#include "include/hitable.h"
#include "include/sphere.h"
#include "include/camera.h"
#include "include/material.h"

int main() {

    // WORLD HITABLE OBJECTS
    auto material_ground = std::make_shared<LAMBERTIAN>(COLOR(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<LAMBERTIAN>(COLOR(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<METAL>(COLOR(0.8, 0.8, 0.8), 0.3);
    auto material_right  = std::make_shared<METAL>(COLOR(0.8, 0.6, 0.2), 1);

    HITABLE_OBJECT_LIST world;
    world.add(std::make_shared<SPHERE>(POINT3(0, 0, -1), 0.5, material_center));
    world.add(std::make_shared<SPHERE>(POINT3(0, -100.5, -1), 100, material_ground));
    world.add(std::make_shared<SPHERE>(POINT3(-1, 0, -1), 0.5, material_left));
    world.add(std::make_shared<SPHERE>(POINT3(1, 0, -1), 0.5, material_right));

    // CAMERA
    CAMERA cam;
    cam.render(world);

    return 0;
}