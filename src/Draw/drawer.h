#pragma once

#include <stack>
#include <memory>

#include "texture.h"
#include "shader.h"
#include "texture_manager.h"
#include "buffer_drawer.h"
#include "frame_buffer.h"

GLIB_NAMESPACE_OPEN

class Drawer {
public:
    Drawer(RendererCore::Window& window);

    void Start();
    void End();

    void BeginBake(std::shared_ptr<FrameBaker> baker, const RendererCore::Rectangle& renderViewport);
    void EndBake();

    void DrawText(const Geom::Text2D& text2D, Shader* shader = nullptr);
    void DrawBakedTexture(const Geom::Mesh& mesh, FrameBaker& fm, Shader* shader = nullptr);
    void DrawMesh(const Geom::Mesh& mesh, const Texture* texture = nullptr, Shader* shader = nullptr);
private:
    void DrawMesh(const Geom::Mesh& mesh, TextureManager& tm, const Texture* texture, Shader* shader);
private:
    RendererCore::Window* m_Window;

    TextureManager m_LinearTexManager;
    TextureManager m_NearestTexManager;

    std::shared_ptr<Framebuffer> m_MainFrameBuffer;
    std::shared_ptr<FrameBaker> m_FontBaker;
    std::stack<std::shared_ptr<FrameBaker>> m_FrameBakers;

    std::shared_ptr<Shader> m_BasicShader;
    std::shared_ptr<Shader> m_BasicFontShader;

    Texture m_BasicTexture = TextureManager::GetBasicTex();
    BufferDrawer m_BufferDrawer;
};

GLIB_NAMESPACE_CLOSE