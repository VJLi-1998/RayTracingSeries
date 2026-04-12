#ifndef BVH 
#define BVH

#include "hitable.h"
#include "interval.h"

#include <algorithm>


class BVH_NODE : public HITABLE_OBJECT {
public:
    BVH_NODE() {}
    BVH_NODE(HITABLE_OBJECT_LIST& list) : BVH_NODE(list.objects(), 0, list.objects().size()) {}
    BVH_NODE(std::vector<std::shared_ptr<HITABLE_OBJECT>>& objects, int start, int end) {
        int axis = random_int(0, 2);
        auto comparator = [axis](const std::shared_ptr<HITABLE_OBJECT>& a, const std::shared_ptr<HITABLE_OBJECT>& b) {
            return a->bounding_box().axis_interval(axis).min() < b->bounding_box().axis_interval(axis).min();
        };

        int object_span = end - start;

        if (object_span == 1) {
            m_left = m_right = objects[start];
        } else if (object_span == 2) {
            m_left = objects[start];
            m_right = objects[start + 1];
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            int mid = start + object_span / 2;
            m_left = std::make_shared<BVH_NODE>(objects, start, mid);
            m_right = std::make_shared<BVH_NODE>(objects, mid, end);
        }

        m_bbox = AABB(m_left->bounding_box(), m_right->bounding_box());
    }

    virtual bool hit(const RAY& r, INTERVAL interval, HIT_RECORD& rec) const override {
        if (!m_bbox.hit(r, interval)) {
            return false;
        }

        bool hit_left = m_left->hit(r, interval, rec);
        bool hit_right = m_right->hit(r, INTERVAL(interval.min(), hit_left ? rec.t : interval.max()), rec);

        return hit_left || hit_right;
    }

    virtual AABB bounding_box() const override {
        return m_bbox;
    }

public:
    std::shared_ptr<HITABLE_OBJECT> m_left;
    std::shared_ptr<HITABLE_OBJECT> m_right;
    AABB m_bbox;
};


#endif // BVH