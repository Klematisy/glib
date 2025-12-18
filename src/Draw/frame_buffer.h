#pragma once

#include <memory>
#include <array>
#include "structs.h"
#include "texture_manager.h"
#include "Geometry/mesh.h"
#include "texture.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

class Framebuffer {
public:
    Framebuffer(RendererCore::Window* window);
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) = delete;

    void BeginRenderCatch();
    void EndRenderCatch();

    DrawBuffer& GetBuffer();
protected:
    RendererCore::Window* m_Window = nullptr;
    DrawBuffer m_ContentsBuffer;
};

class FrameBaker {
public:
    FrameBaker(RendererCore::Window* window);

    void BeginRenderCatch();
    void EndRenderCatch();

    const Texture& GetRenderTexture() const;
    std::shared_ptr<TextureManager> GetTextureManager() const;
private:
    void UpdateData();
private:
    std::shared_ptr<TextureManager> m_TexManager;
    RendererCore::Window* m_Window = nullptr;
    Texture m_RenderTexture;

    RendererCore::Rectangle m_BakeField;
    RendererCore::Rectangle m_Viewport;
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;
};

GLIB_NAMESPACE_CLOSE