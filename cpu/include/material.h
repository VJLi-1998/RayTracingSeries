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

class DIELECTRIC : public MATERIAL {
public:
    DIELECTRIC(float refraction_index) : m_ir(refraction_index) {}

    bool scatter(const RAY& r_in, const HIT_RECORD& rec, 
                COLOR& attenuation, RAY& scattered) const override {
        attenuation = COLOR(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0 / m_ir) : m_ir;

        VEC3 unit_direction = unit_vector(r_in.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        float sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        VEC3 direction;
        if (cannot_refract || reflectance_ratio(cos_theta, refraction_ratio) > random_float()) {
            direction = get_reflect_vec(unit_direction, rec.normal);
        } else {
            direction = get_refract_vec(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = RAY(rec.p, direction);
        return true;
    }
private:
    static float reflectance_ratio(float cosine, float ref_idx) {
        // Use Schlick's approximation for reflectance.
        float r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }

private:
    float m_ir; // Index of Refraction

};

#endif // MATERIAL_H