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
    u0 Init();

    GAPI::ImageInfo image;
private:
    void syncTextureWithImage();
    void StartBake();
    void EndBake();

    GAPI::FramebufferPTR m_FB;
    GAPI::RenderbufferPTR m_RB;
    GAPI::TextureArrayPTR m_Texture;
};

VLADLIB_NAMESPACE_CLOSE
