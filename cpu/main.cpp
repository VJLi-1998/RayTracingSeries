#include <iostream>

#include "include/color.h"
#include "include/vec3.h"
#include "include/ray.h"

bool hit_sphere(const POINT3& center, float radius, const RAY& r) {
    VEC3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto b = -2 * dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

COLOR ray_color(const RAY& r) {
    if (hit_sphere(POINT3(0,0,-1), 0.5, r)) {
        return COLOR(1, 0, 0);
    }

    VEC3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*COLOR(1.0, 1.0, 1.0) + t*COLOR(0.5, 0.7, 1.0);
}

int main() {
    // IMAGE
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 1080;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    // CAMERA
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (1.0 * image_width / image_height);

    POINT3 camera_center = POINT3(0, 0, 0);
    
    // VIEWPORT
    VEC3 viewport_h = VEC3(viewport_width, 0, 0);
    VEC3 viewport_v = VEC3(0, -viewport_height, 0);

    VEC3 pixel_delta_h = viewport_h / image_width;
    VEC3 pixel_delta_v = viewport_v / image_height;

    VEC3 viewport_upleft = camera_center - VEC3(0,0,focal_length) - viewport_h/2 - viewport_v/2;  
    VEC3 pixel_upleft = viewport_upleft + (pixel_delta_h + pixel_delta_v) / 2;

    // RENDER
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << image_height - j - 1 << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_cur = pixel_upleft + i * pixel_delta_h + j * pixel_delta_v;
            auto ray_direction = pixel_cur - camera_center;
            RAY r(camera_center, ray_direction);

            COLOR pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "Done.\n";
    return 0;
}