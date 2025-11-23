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

    virtual void BeginRenderCatch();
    virtual void EndRenderCatch();

    DrawBuffer& GetBuffer();
    virtual glm::mat4 GetProjMatrix() const;
protected:
    RendererCore::Window* m_Window = nullptr;
    DrawBuffer m_ContentsBuffer;
};

class FrameBaker : public Framebuffer {
public:
    FrameBaker(RendererCore::Window* window);

    void BeginRenderCatch(const RendererCore::Rectangle& rect);
    void EndRenderCatch() override;

    const Texture& GetRenderTexture() const;
    glm::mat4 GetProjMatrix() const override;
    const RendererCore::Rectangle& GetViewport() const;
    std::shared_ptr<TextureManager> GetTextureManager() const;
private:
    void UpdateData();
private:
    std::shared_ptr<TextureManager> m_TexManager;
    Texture m_RenderTexture;

    RendererCore::Rectangle m_Viewport;
    RendererCore::Rectangle m_LastViewport;
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;
};

GLIB_NAMESPACE_CLOSE