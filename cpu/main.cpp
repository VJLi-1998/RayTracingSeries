#include <iostream>

#include "include/common_header.h"
#include "include/hitable.h"
#include "include/sphere.h"
#include "include/camera.h"

int main() {

    // WORLD HITABLE OBJECTS
    HITABLE_OBJECT_LIST world;
    world.add(std::make_shared<SPHERE>(POINT3(0, 0, -1), 0.5));
    world.add(std::make_shared<SPHERE>(POINT3(0, -100.5, -1), 100));
    
    // CAMERA
    CAMERA cam;
    cam.render(world);

    return 0;
}