#pragma once

#include <vector>

namespace GlCore {

    struct LayoutData {
        uint32_t size = 0;
        uint32_t offset = 0;
        GLenum type = GL_INT;
    };

    class VertexArrayLayout {
    public:
        template<class T>
        void Add(uint32_t i) {
            m_Layouts.push_back({i, m_Offset});
            m_Offset += i * sizeof(T);
        }

        template<>
        void Add<float>(uint32_t i) {
            m_Layouts.push_back({i, m_Offset, GL_FLOAT});
            m_Offset += i * sizeof(float);
        }

        template<>
        void Add<unsigned int>(uint32_t i) {
            m_Layouts.push_back({i, m_Offset, GL_UNSIGNED_INT});
            m_Offset += i * sizeof(float);
        }

        template<>
        void Add<unsigned char>(uint32_t i) {
            m_Layouts.push_back({i, m_Offset, GL_UNSIGNED_BYTE});
            m_Offset += i;
        }

        const std::vector<LayoutData> &GetLayouts() const {
            return m_Layouts;
        }

        int GetFullOffset() const {
            return static_cast<int>(m_Offset);
        }

    private:
        uint32_t m_Offset = 0;
        std::vector<LayoutData> m_Layouts;
    };

}