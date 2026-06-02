#pragma once

#include <vector>
#include "Graphics/GraphicsAPI/graphics_api.h"

namespace RendererCore {
    struct LayoutData {
        uint32_t size = 0;
        uint32_t offset = 0;
        GAPI::API_TYPE type = GAPI::API_TYPE::INT;
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
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::FLOAT});
            m_Offset += i * sizeof(float);
        }

        template<>
        void Add<unsigned int>(uint32_t i) {
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::UINT});
            m_Offset += i * sizeof(unsigned int);
        }

        template<>
        void Add<unsigned char>(uint32_t i) {
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::UCHAR});
            m_Offset += i;
        }

        const std::vector<LayoutData>& GetLayouts() const {
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
