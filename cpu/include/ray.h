#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class RAY
{
    public:
        RAY() {}
        RAY(const POINT3& origin, const VEC3& direction)
            : m_origin(origin), m_direction(direction) {}

        const POINT3& origin() const { return m_origin; }
        const VEC3& direction() const { return m_direction; }

        POINT3 at(float t) const {
            return m_origin + t * m_direction;
        }
    private:
        POINT3 m_origin;
        VEC3 m_direction;
};

#endif // RAY_H