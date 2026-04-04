#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

using COLOR = VEC3;    // RGB color

void write_color(std::ostream &out, const COLOR& pixel_color) {
    int ir = static_cast<int>(255.999 * pixel_color.x());
    int ig = static_cast<int>(255.999 * pixel_color.y());
    int ib = static_cast<int>(255.999 * pixel_color.z());

    out << ir << ' ' << ig << ' ' << ib << '\n';
}


#endif // COLOR_H