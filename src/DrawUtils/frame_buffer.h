#pragma once

#include <memory>
#include <array>
#include "structs.h"
#include "texture_atlas.h"
#include "Geometry/mesh.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

//class FrameBaker {
//public:
//    FrameBaker(RendererCore::Window* window);
//
//    void BeginRenderCatch();
//    void EndRenderCatch();
//
//    const Texture& GetRenderTexture() const;
//private:
//    void UpdateData();
//private:
//    RendererCore::Window* m_Window = nullptr;
//    Texture m_RenderTexture;
//
//    RendererCore::Rectangle m_BakeField;
//    RendererCore::Rectangle m_Viewport;
//    RendererCore::Framebuffer m_FB;
//    RendererCore::Renderbuffer m_RB;
//};

GLIB_NAMESPACE_CLOSE