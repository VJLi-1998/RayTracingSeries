#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class SPHERE : public HITABLE_OBJECT {
    public:
        SPHERE() {}
        SPHERE(const POINT3& cen, float r, std::shared_ptr<MATERIAL> mat) : 
                    m_center(cen), m_radius(r), m_material(mat) {
            auto rvec = VEC3(r, r, r);
            m_aabb = AABB(m_center - rvec, m_center + rvec);
        }

        virtual bool hit(const RAY& r, INTERVAL interval, HIT_RECORD& rec) const override {
            POINT3 oc = m_center - r.origin();
            float a = r.direction().length_squared();
            float half_b = dot(oc, r.direction());
            float c = oc.length_squared() - m_radius*m_radius;
            float discriminant = half_b * half_b - a*c;

            if (discriminant < 0) return false;

            float root = (half_b - std::sqrt(discriminant)) / a;
            if (!interval.surrounds(root)) {
                root = (half_b + std::sqrt(discriminant)) / a;
                if (!interval.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            POINT3 outward_normal = (rec.p - m_center) / m_radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = m_material;
            return true;
        }

        virtual AABB bounding_box() const override {
            return m_aabb;
        }

    public:
        POINT3 m_center;
        float m_radius;
        std::shared_ptr<MATERIAL> m_material;
        AABB m_aabb;
};

#endif // SPHERE_H