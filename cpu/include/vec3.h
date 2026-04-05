#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class VEC3 {
private:
    float m_x, m_y, m_z;
public:

    VEC3() : m_x(0), m_y(0), m_z(0) {}
    VEC3(float e0, float e1, float e2) : m_x(e0), m_y(e1), m_z(e2) {}

    float x() const { return m_x; }
    float y() const { return m_y; }
    float z() const { return m_z; }

    inline VEC3 operator-() const { return VEC3(-m_x, -m_y, -m_z); }
    inline float operator[](int i) const { return i == 0 ? m_x : (i == 1 ? m_y : m_z); }
    inline float& operator[](int i) { return i == 0 ? m_x : (i == 1 ? m_y : m_z); }

    inline VEC3& operator+=(const VEC3 &v) {
        m_x += v.m_x;
        m_y += v.m_y;
        m_z += v.m_z;
        return *this;
    }

    inline VEC3& operator*=(const float t) {
        m_x *= t;
        m_y *= t;
        m_z *= t;
        return *this;
    }

    inline VEC3& operator/=(const float t) {
        return *this *= 1 / t;
    }

    float length() const {
        return std::sqrt(length_squared());
    }

    float length_squared() const {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    bool near_zero() const {
        const float threshold = 1e-8;
        return (std::fabs(m_x) < threshold) && (std::fabs(m_y) < threshold) && (std::fabs(m_z) < threshold);
    }

    static VEC3 random_vec3() {
        return VEC3(random_float(), random_float(), random_float());
    }

    static VEC3 random_vec3(float min, float max) {
        return VEC3(random_float(min, max), random_float(min, max), random_float(min, max));
    }

};

using POINT3 = VEC3;   // 3D point

inline std::ostream& operator<<(std::ostream &out, const VEC3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline VEC3 operator+(const VEC3 &u, const VEC3 &v) {
    return VEC3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline VEC3 operator-(const VEC3 &u, const VEC3 &v) {
    return VEC3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline VEC3 operator*(const VEC3 &u, const VEC3 &v) {
    return VEC3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline VEC3 operator*(float t, const VEC3 &v) {
    return VEC3(t * v.x(), t * v.y(), t * v.z());
}

inline VEC3 operator/(const VEC3 &v, float t) {
    return (1 / t) * v;
}

inline float dot(const VEC3 &u, const VEC3 &v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline VEC3 cross(const VEC3 &u, const VEC3 &v) {
    return VEC3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

inline VEC3 unit_vector(const VEC3 &v) {
    return v / v.length();
}

inline VEC3 reject_sample_vec() {
    while (true) {
        VEC3 sample = VEC3::random_vec3(-1, 1);
        float length_squared = sample.length_squared();
        if (length_squared >= 1 && length_squared < 1e-10) continue;
        return sample / std::sqrt(length_squared);
    }
}

inline VEC3 get_random_vec_on_hemisphere(const VEC3& normal) {
    VEC3 in_unit_sphere = reject_sample_vec();
    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

inline VEC3 get_reflect_vec(const VEC3& v, const VEC3& n) {
    return v - 2 * dot(v, n) * n;
}

inline VEC3 get_refract_vec(const VEC3& uv, const VEC3& n, float etai_over_etat) {
    float cos_theta = fmin(dot(-uv, n), 1.0);
    VEC3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    VEC3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif // VEC3_H