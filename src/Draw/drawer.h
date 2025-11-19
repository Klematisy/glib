#pragma once

#include <stack>
#include <memory>

#include "texture.h"
#include "shader.h"
#include "texture_manager.h"
#include "framebuffer_drawer.h"
#include "frame_buffer.h"

GLIB_NAMESPACE_OPEN

class Drawer {
public:
    Drawer(RendererCore::Window& window);

    void Start();
    void End();

    void DrawText(const Geom::Text2D& text2D, const Color& color, Shader* shader = nullptr);
    void DrawMesh(const Geom::Mesh& mesh, const Color& color, const Texture* texture = nullptr, Shader* shader = nullptr);
private:
    Buffer m_AllBuffer;

    std::stack<Framebuffer> m_FB;
    Framebuffer m_FontFramebuffer;
    FramebufferDrawer m_FD;

    TextureManager m_LinearTexManager;
    TextureManager m_NearestTexManager;

    std::shared_ptr<Shader> m_BasicShader;
    std::shared_ptr<Shader> m_BasicFontShader;

    Texture m_BasicTexture;
};

GLIB_NAMESPACE_CLOSE