#include "glm.hpp"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

bool vec3Equal(const glm::vec3& v1, const glm::vec3& v2) {
    constexpr float eps = 1e-5f;
    return  fabsf(v1.x - v2.x) < eps &&
            fabsf(v1.y - v2.y) < eps &&
            fabsf(v1.z - v2.z) < eps;
}

class Polygon4 {
public:
    void Clear() {
        m_FilledCount = 0;
    }

    const glm::vec3& operator[](uint32_t i) const {
        assert(i < m_FilledCount);
        return m_Points[i];
    }

    void AddPoint(const glm::vec3& point) {
        if (m_FilledCount == 4) {
            for (uint8_t i = 0; i < 3; i++) {
                m_Points[i] = m_Points[i + 1];
            }
            m_Points[3] = point;
        } else {
            m_Points[m_FilledCount++] = point;
        }
    }

    bool operator==(const Polygon4& other) const {
        uint8_t collision = 0;

        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                if (vec3Equal(m_Points[i], other.m_Points[j])) {
                    collision++;
                    break;
                }
            }
        }

        return collision == 4;
    }
private:
    std::array<glm::vec3, 4> m_Points{};
    uint8_t m_FilledCount = 0;
};

VLADLIB_NAMESPACE_CLOSE