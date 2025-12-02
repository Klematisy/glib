#include "atlas.h"

GLIB_NAMESPACE_USING;

Atlas::Atlas(const Texture& tex)
    : m_Texture(&tex)
{}

void Atlas::SetSrcSize(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
}

void Atlas::SetXGap(uint32_t rightGap, uint32_t leftGap) {
    m_RightGap = rightGap;
    m_LeftGap = leftGap;
}

void Atlas::SetYGap(uint32_t UpGap, uint32_t DownGap) {
    m_UpGap = UpGap;
    m_DownGap = DownGap;
}

void Atlas::SetTexCoords(Geom::Mesh& mesh, int x, int y) {
    uint32_t xGap = (m_LeftGap + m_RightGap + m_Width) * x;
    uint32_t yGap = (m_DownGap + m_UpGap + m_Height) * y;

    mesh.SetUV(
        {
            (float) (m_LeftGap + xGap)           / (float) m_Texture->GetWidth(), (float) (m_UpGap + yGap)            / (float) m_Texture->GetHeight(),
            (float) (m_LeftGap + xGap)           / (float) m_Texture->GetWidth(), (float) (m_UpGap + yGap + m_Height) / (float) m_Texture->GetHeight(),
            (float) (m_LeftGap + xGap + m_Width) / (float) m_Texture->GetWidth(), (float) (m_UpGap + yGap + m_Height) / (float) m_Texture->GetHeight(),
            (float) (m_LeftGap + xGap + m_Width) / (float) m_Texture->GetWidth(), (float) (m_UpGap + yGap)            / (float) m_Texture->GetHeight()
        }
    );
}