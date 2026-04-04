#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"
#include "interval.h"

using COLOR = VEC3;    // RGB color

void write_color(std::ostream &out, const COLOR& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    static const INTERVAL intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif // COLOR_H