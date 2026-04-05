#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"
#include "color.h"
#include "vec3.h"

// 1. Produce a scattered ray (or say it absorbed the incident ray).
// 2. If scattered, say how much the ray should be attenuated.

class MATERIAL {
public:
    virtual ~MATERIAL() = default;

    virtual bool scatter(const RAY& r_in, const HIT_RECORD& rec, 
                COLOR& attenuation, RAY& scattered) const {
        return false;
    }
};

class LAMBERTIAN : public MATERIAL {
public:
    LAMBERTIAN(const COLOR& a) : m_albedo(a) {}

    virtual bool scatter(const RAY& r_in, const HIT_RECORD& rec, 
                COLOR& attenuation, RAY& scattered) const override {
        VEC3 scatter_direction = rec.normal + get_random_vec_on_hemisphere(rec.normal);
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }
        scattered = RAY(rec.p, scatter_direction);
        attenuation = m_albedo;
        return true;
    }

private:
    COLOR m_albedo;
};

class METAL : public MATERIAL {
public:
    METAL(const COLOR& a, float fuzz) : m_albedo(a), m_fuzz(fuzz < 1 ? fuzz : 1.0) {}

    virtual bool scatter(const RAY& r_in, const HIT_RECORD& rec, 
                COLOR& attenuation, RAY& scattered) const override {
        VEC3 reflected = get_reflect_vec(r_in.direction(), rec.normal);

        reflected = unit_vector(reflected) + m_fuzz * reject_sample_vec();

        scattered = RAY(rec.p, reflected);
        attenuation = m_albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
private:
    COLOR m_albedo;
    float m_fuzz;
};

#endif // MATERIAL_H