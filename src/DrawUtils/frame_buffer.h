#pragma once

#include <memory>
#include <array>
#include "structs.h"
#include "texture_atlas.h"
#include "Geometry/mesh.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

class FrameBaker {
public:
    FrameBaker();

    void StartBake();
    void EndBake();

    const RendererCore::Framebuffer& GetFrameBuffer() const;
    const rc::ImageInfo& GetImage() const;
private:
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;

    RendererCore::ImageInfo m_Image = rc::ImageInfo(3000, 3000, 4, nullptr);
};

GLIB_NAMESPACE_CLOSE