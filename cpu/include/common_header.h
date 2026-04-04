#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degrees2radians(float degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif // COMMON_HEADER_H