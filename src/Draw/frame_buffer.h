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
    virtual glm::mat4 GetProjMatrix();
protected:
    RendererCore::Window* m_Window = nullptr;
    DrawBuffer m_ContentsBuffer;
};

class FrameBaker : public Framebuffer {
public:
    FrameBaker(RendererCore::Window* window);

    void BeginRenderCatch() override;
    void EndRenderCatch() override;

    void SetRenderTexture(TextureManager& tm, const Texture& tex);
    glm::mat4 GetProjMatrix() override;
private:
    void UpdateData();
private:
    int m_Width = 0, m_Height = 0;
    int m_LastRenderWidth = 0, m_LastRenderHeight = 0;
    RendererCore::Framebuffer m_FB;
    RendererCore::Renderbuffer m_RB;
};

GLIB_NAMESPACE_CLOSE