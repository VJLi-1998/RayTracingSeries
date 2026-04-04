#ifndef CAMERA_H
#define CAMERA_H

#include "hitable.h"
#include "common_header.h"

class CAMERA {
public:
    CAMERA() {}

    void render(const HITABLE_OBJECT& world) {
        initialize();

        // RENDER
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << image_height - j - 1 << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto pixel_cur = pixel_upleft + i * pixel_delta_h + j * pixel_delta_v;
                auto ray_direction = pixel_cur - camera_center;
                RAY r(camera_center, ray_direction);

                COLOR pixel_color = COLOR(0, 0, 0);

                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    RAY sample_ray = get_sample_ray(i, j);
                    pixel_color += ray_color(sample_ray, world);
                }
                write_color(std::cout, pixel_color / samples_per_pixel);
            }
        }

        std::clog << "Done.\n";
    }

private:
    int samples_per_pixel = 1;
    int image_width, image_height;
    POINT3 camera_center;
    VEC3 pixel_delta_h, pixel_delta_v;
    VEC3 pixel_upleft;

    void initialize() {
        // IMAGE
        const float aspect_ratio = 16.0 / 9.0;
        image_width = 400;
        image_height = static_cast<int>(image_width / aspect_ratio);

        // CAMERA
        camera_center = POINT3(0, 0, 0);

        float focal_length = 1.0;

        // VIEWPORT
        float viewport_height = 2.0;
        float viewport_width = viewport_height * (1.0 * image_width / image_height);

        VEC3 viewport_h = VEC3(viewport_width, 0, 0);
        VEC3 viewport_v = VEC3(0, -viewport_height, 0);

        pixel_delta_h = viewport_h / image_width;
        pixel_delta_v = viewport_v / image_height;

        VEC3 viewport_upleft = camera_center - VEC3(0,0,focal_length) - viewport_h/2 - viewport_v/2;  
        pixel_upleft = viewport_upleft + (pixel_delta_h + pixel_delta_v) / 2;
    }

    COLOR ray_color(const RAY& r, const HITABLE_OBJECT& world) {
        HIT_RECORD result;
        if (world.hit(r, INTERVAL(0, infinity), result)) {
            return 0.5 * COLOR(result.normal.x() + 1, result.normal.y() + 1, result.normal.z() + 1);
        }

        VEC3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*COLOR(1.0, 1.0, 1.0) + t*COLOR(0.5, 0.7, 1.0);
    }

    RAY get_sample_ray(const int i, const int j) {
        float u = (i + random_float() - 0.5);
        float v = (j + random_float() - 0.5);
        VEC3 ray_direction = pixel_upleft + u * pixel_delta_h + v * pixel_delta_v - camera_center;
        return RAY(camera_center, ray_direction);
    }
};

#endif // CAMERA_H