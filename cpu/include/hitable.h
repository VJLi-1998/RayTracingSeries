#ifndef HITALBE_H
#define HITALBE_H

#include  "ray.h"

#include <vector>
#include <memory>

class MATERIAL;

class HIT_RECORD {
    public:
        POINT3 p;
        VEC3 normal;
        float t;
        bool front_face;
        std::shared_ptr<MATERIAL> mat;

    public:
        void set_face_normal(const RAY& r, const VEC3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class HITABLE_OBJECT {
    public:
        virtual bool hit(const RAY& r, INTERVAL interval, HIT_RECORD& rec) const = 0;
};

class HITABLE_OBJECT_LIST : public HITABLE_OBJECT {
    public:
        HITABLE_OBJECT_LIST() {}
        HITABLE_OBJECT_LIST(std::shared_ptr<HITABLE_OBJECT> object) { add(object); }
        HITABLE_OBJECT_LIST(std::vector<std::shared_ptr<HITABLE_OBJECT>> objects) : m_objects(objects) {}

        void clear() { m_objects.clear(); }
        void add(std::shared_ptr<HITABLE_OBJECT> object) { m_objects.push_back(object); }

        virtual bool hit(const RAY& r, INTERVAL interval, HIT_RECORD& rec) const override {
            HIT_RECORD temp_rec;
            bool hit_anything = false;
            float closest_so_far = interval.max();

            for (const auto& object : m_objects) {
                if (object->hit(r, INTERVAL(interval.min(), closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

    private:
        std::vector<std::shared_ptr<HITABLE_OBJECT>> m_objects;
};

#endif // HITALBE_H