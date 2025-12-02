#pragma once

#include "environment.h"
#include "Geometry/mesh.h"
#include "Draw/texture.h"

GLIB_NAMESPACE_OPEN

class Atlas {
public:
    Atlas(const Texture& tex);
    void SetSrcSize(uint32_t width, uint32_t height);
    void SetXGap(uint32_t rightGap, uint32_t leftGap);
    void SetYGap(uint32_t UpGap, uint32_t DownGap);
    void SetTexCoords(Geom::Mesh& mesh, int x, int y);
private:
    const Texture* m_Texture = nullptr;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    uint32_t m_RightGap = 0;
    uint32_t m_LeftGap = 0;
    uint32_t m_UpGap = 0;
    uint32_t m_DownGap = 0;
};

GLIB_NAMESPACE_CLOSE