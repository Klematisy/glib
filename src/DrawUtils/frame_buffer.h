#pragma once

#include <memory>
#include <array>
#include "structs.h"
#include "texture_atlas.h"
#include "Geometry/mesh.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"
#include "DrawUtils/texture_atlas.h"

GLIB_NAMESPACE_OPEN

class FrameBaker {
public:
    FrameBaker();

    void StartBake();
    void EndBake();

    const RendererCore::Framebuffer& GetFrameBuffer() const;
    RendererCore::ImageInfo& GetImage();
private:
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;

    RendererCore::ImageInfo m_Image = RendererCore::ImageInfo(TexArrElInfo::WIDTH_MAX_SIZE, TexArrElInfo::HEIGHT_MAX_SIZE, 4, nullptr);
};

GLIB_NAMESPACE_CLOSE