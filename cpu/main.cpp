#include <iostream>

#include "include/color.h"
#include "include/vec3.h"

int main() {
    // IMAGE
    int image_width = 256;
    int image_height = 256;

    // RENDER
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << image_height - j - 1 << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;
            COLOR pixel_color(r, g, b);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "Done.\n";
    return 0;
}