#ifndef CAMERA_H
#define CAMERA_H

#include "hitable.h"
#include "material.h"
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
                    pixel_color += ray_color(sample_ray, max_depth, world);
                }
                write_color(std::cout, pixel_color / samples_per_pixel);
            }
        }

        std::clog << "Done.\n";
    }

    void set_look_from(const POINT3& look_from) { this->look_from = look_from; }
    void set_look_at(const POINT3& look_at) { this->look_at = look_at; }
    void set_vfov(float vfov) { this->vfov = vfov; }
    void set_samples_per_pixel(int samples) { this->samples_per_pixel = samples; }
    void set_max_depth(int depth) { this->max_depth = depth; }
    void set_image_width(int width) { this->image_width = width; }
    void set_defocus_angle(float angle) { this->defocus_angle = angle; }
    void set_focus_distance(float distance) { this->focus_distance = distance; }


private:
    int samples_per_pixel = 10;
    int max_depth = 50;
    int image_width = 400, image_height;
    float vfov = 90; // vertical field of view in degrees
    float defocus_angle = 0;
    float focus_distance = 10;
    POINT3 camera_center;
    POINT3 look_from, look_at;
    VEC3 pixel_delta_h, pixel_delta_v;
    VEC3 pixel_upleft;
    VEC3 u,v,w;
    VEC3 view_up = VEC3(0, 1, 0);
    VEC3 defocus_disk_u, defocus_disk_v;

    void initialize() {
        // IMAGE
        const float aspect_ratio = 16.0 / 9.0;
        image_height = static_cast<int>(image_width / aspect_ratio);

        // CAMERA
        camera_center = look_from;

        float theta = degrees2radians(vfov);

        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        // VIEWPORT
        float viewport_height = std::tan(theta/2) * focus_distance * 2;
        float viewport_width = viewport_height * (1.0 * image_width / image_height);

        VEC3 viewport_h = viewport_width * u;
        VEC3 viewport_v = viewport_height * -v;

        pixel_delta_h = viewport_h / image_width;
        pixel_delta_v = viewport_v / image_height;

        VEC3 viewport_upleft = camera_center - focus_distance * w - viewport_h/2 - viewport_v/2;  
        pixel_upleft = viewport_upleft + (pixel_delta_h + pixel_delta_v) / 2;

        // 
        float defocus_radius = std::tan(degrees2radians(defocus_angle / 2)) * focus_distance;
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    COLOR ray_color(const RAY& r, int depth, const HITABLE_OBJECT& world) {
        if (depth <= 0) {
            return COLOR(0, 0, 0);
        }

        HIT_RECORD result;
        if (world.hit(r, INTERVAL(0.001, infinity), result)) {
            RAY scattered;
            COLOR attenuation;
            if (result.mat->scatter(r, result, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return COLOR(0, 0, 0);
            // VEC3 diffuse_reflection_direction = result.normal + get_random_vec_on_hemisphere(result.normal);
            // return 0.5 * ray_color(RAY(result.p, diffuse_reflection_direction), depth-1, world);
        }

        VEC3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*COLOR(1.0, 1.0, 1.0) + t*COLOR(0.5, 0.7, 1.0);
    }
    
    POINT3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
    }

    RAY get_sample_ray(const int i, const int j) {
        float tmp_i = (i + random_float() - 0.5);
        float tmp_j = (j + random_float() - 0.5);

        VEC3 ray_origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();
        VEC3 ray_direction = pixel_upleft + tmp_i * pixel_delta_h + tmp_j * pixel_delta_v - ray_origin;
        return RAY(ray_origin, ray_direction);
    }
};

#endif // CAMERA_H