#pragma once

#include <memory>
#include <array>
#include "structs.h"
#include "texture_atlas.h"
#include "Geometry/mesh.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"
#include "DrawUtils/texture_atlas.h"

VLADLIB_NAMESPACE_OPEN

class SceneRenderer;

class FrameBaker {
    friend SceneRenderer;
public:
    FrameBaker();

    RendererCore::ImageInfo& GetImage();
private:
    void syncTextureWithImage();
    void StartBake();
    void EndBake();

    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;

    RendererCore::ImageInfo m_Image {600, 600, 4, nullptr};
    RendererCore::TextureArray m_Texture;
};

VLADLIB_NAMESPACE_CLOSE