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
    FrameBaker(RendererCore::Window* window);

    void StartBake();
    void EndBake();

    const RendererCore::Framebuffer& GetFrameBuffer() const;
private:
    RendererCore::Window* m_Window = nullptr;
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;
};

GLIB_NAMESPACE_CLOSE