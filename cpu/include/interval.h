#ifndef INTERVAL_H
#define INTERVAL_H  

#include "common_header.h"

class INTERVAL {
    public:
        INTERVAL() : m_min(0), m_max(0) {}
        INTERVAL(float min, float max) : m_min(min), m_max(max) {}

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
    
    public:
        static const INTERVAL EMPTY;
        static const INTERVAL UNIVERSE;

    private:
        float m_min;
        float m_max;
};

const INTERVAL INTERVAL::EMPTY    = INTERVAL(+infinity, -infinity);
const INTERVAL INTERVAL::UNIVERSE = INTERVAL(-infinity, +infinity);

#endif // INTERVAL_H