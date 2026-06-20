#pragma once

#include <memory>
#include <array>
#include "GraphicsAPI/graphics_api.h"
#include "structs.h"
#include "texture_atlas.h"
#include "Geometry/mesh.h"

#include "GraphicsAPI/window.h"
#include "DrawUtils/texture_atlas.h"

VLADLIB_NAMESPACE_OPEN

class SceneRenderer;

class FrameBaker {
    friend SceneRenderer;
public:
    FrameBaker();

    GAPI::ImageInfo image {1, 1, 1, nullptr};
private:
    void syncTextureWithImage();
    void StartBake();
    void EndBake();

    std::shared_ptr<GAPI::Framebuffer> m_FB = GAPI::createFramebuffer();
    std::shared_ptr<GAPI::Renderbuffer> m_RB = GAPI::createRenderbuffer();

    std::shared_ptr<GAPI::TextureArray> m_Texture = GAPI::createTexutreArray();
};

VLADLIB_NAMESPACE_CLOSE
