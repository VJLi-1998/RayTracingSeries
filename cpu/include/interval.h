#ifndef INTERVAL_H
#define INTERVAL_H  

#include <algorithm>

#include "vec3.h"
#include "ray.h"

class INTERVAL {
    public:
        INTERVAL() : m_min(0), m_max(0) {}
        INTERVAL(float min, float max) : m_min(min), m_max(max) {}
        INTERVAL(const INTERVAL&a, const INTERVAL& b) : m_min(std::min(a.min(), b.min())), m_max(std::max(a.max(), b.max())) {}

        float min() const { return m_min; }
        float max() const { return m_max; }

        float size() const { return m_max - m_min; }

        bool contains(float value) const {
            return value >= m_min && value <= m_max;
        }

        bool surrounds(float value) const {
            return m_min < value && m_max > value;
        }

        float clamp(float value) const {
            if (value < m_min) return m_min;
            if (value > m_max) return m_max;
            return value;
        }
    
        INTERVAL expand(double delta) const {
            auto padding = delta/2;
            return INTERVAL(m_min - padding, m_max + padding);
        }
    
    public:
        static const INTERVAL EMPTY;
        static const INTERVAL UNIVERSE;

    private:
        float m_min;
        float m_max;
};

class AABB {
public:
    AABB() {}
    AABB(const INTERVAL& x, const INTERVAL& y, const INTERVAL& z) : m_x(x), m_y(y), m_z(z) {}
    AABB(const POINT3& a, const POINT3& b) {
        m_x = INTERVAL(std::min(a.x(), b.x()), std::max(a.x(), b.x()));
        m_y = INTERVAL(std::min(a.y(), b.y()), std::max(a.y(), b.y()));
        m_z = INTERVAL(std::min(a.z(), b.z()), std::max(a.z(), b.z()));
    }
    AABB(const AABB& a, const AABB& b) {
        m_x = INTERVAL(a.x(), b.x());
        m_y = INTERVAL(a.y(), b.y());
        m_z = INTERVAL(a.z(), b.z());
    }

    const INTERVAL& axis_interval(int axis) const {
        if (axis == 0) return m_x;
        if (axis == 1) return m_y;
        return m_z;
    }

    bool hit(const RAY& r, INTERVAL ray_t) const {
        const POINT3& origin = r.origin();
        const VEC3& direction = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const INTERVAL& interval = axis_interval(axis);
            float invD = 1.0f / direction[axis];
            float t0 = (interval.min() - origin[axis]) * invD;
            float t1 = (interval.max() - origin[axis]) * invD;
            
            if (t0 < t1) {
                ray_t = INTERVAL(std::max(t0, ray_t.min()), std::min(t1, ray_t.max()));
            } else {
                ray_t = INTERVAL(std::max(t1, ray_t.min()), std::min(t0, ray_t.max()));
            }

            if (ray_t.max() <= ray_t.min())
                return false;

        }
        return true;
    }

    const INTERVAL& x() const { return m_x; }
    const INTERVAL& y() const { return m_y; }
    const INTERVAL& z() const { return m_z; }
private:
    INTERVAL m_x, m_y, m_z;

};

const INTERVAL INTERVAL::EMPTY    = INTERVAL(+infinity, -infinity);
const INTERVAL INTERVAL::UNIVERSE = INTERVAL(-infinity, +infinity);

#endif // INTERVAL_H