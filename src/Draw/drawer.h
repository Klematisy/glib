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

    void BeginBake(FrameBaker* baker);
    void EndBake();

    void SetCamera(Camera* camera);
    const Camera* GetCamera() const;

    void DrawText(const Geom::Text2D& text2D, Shader* shader = nullptr);
    void DrawMesh(const Geom::Mesh& mesh, const Texture* texture = nullptr, Shader* shader = nullptr, GAPI::TEXTURE_PARAM textureParam = GAPI::TEXTURE_PARAM::NEAREST);

    void DrawBakedTexture(const Geom::Mesh& mesh, FrameBaker& fm, Shader* shader = nullptr);
    void DrawMeshWithTextureManager(const Geom::Mesh& mesh, const Texture* texture, TextureManager* tm, Shader* shader);
private:
    RendererCore::Window* m_Window;
    TextureManager m_LinearTexManager;
    TextureManager m_NearestTexManager;

    std::shared_ptr<Framebuffer> m_MainFrameBuffer;
    std::shared_ptr<FrameBaker> m_MainFrameBaker;
    std::shared_ptr<FrameBaker> m_FontBaker;
    std::stack<FrameBaker*> m_FrameBakers;
    std::stack<RendererCore::Rectangle> m_Viewports;

    std::shared_ptr<Shader> m_BasicShader;
    std::shared_ptr<Shader> m_BasicFontShader;

    Texture m_BasicTexture = TextureManager::GetBasicTex();
    BufferDrawer m_BufferDrawer;
    
    std::unique_ptr<OrthographicCamera> m_StandardOCamera;
    std::unique_ptr<PerspectiveCamera> m_StandardPCamera;

    Camera* m_Camera = nullptr;
};

GLIB_NAMESPACE_CLOSE