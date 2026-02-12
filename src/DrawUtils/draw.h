#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "Graphics/RendererCore/renderer.h"
#include "Geometry/camera.h"
#include "Geometry/entity.h"

#include "texture_manager.h"
#include "frame_buffer.h"
#include "shader.h"
#include "batch.h"

GLIB_NAMESPACE_OPEN

struct Vertex {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec3 uv = glm::vec3(1.0f);
};

class EntityToVerticesEvaluator {
public:
    static std::vector<Vertex> Convert(const Geom::Entity& e, const TexInfoConstRef& texInfo);
};

class Draw {
public:
    Draw(RendererCore::Window* window);

    void StartDraw();
    void EndDraw();
    void DrawEntity(const Geom::Entity& e);

    void UseCamera(Camera* cam);
    Camera* GetCamera() const;

    void StartBake(FrameBaker& fm);
    void EndBake();

    void TieImageAndFrameBuffer(RendererCore::ImageInfo& image, FrameBaker& fm);
private:
    void FlushBatch();
private:
    RendererCore::GraphicsBuffer m_GB;
    RendererCore::ImageInfo m_StandardTex;
    TextureManager m_TexManager;
    RendererCore::Renderer m_Renderer;
    Batch<Vertex> m_Batch;

    Camera* m_Camera = nullptr;
    RendererCore::Window* m_Window = nullptr;

    RendererCore::TextureParameters m_LastTexParams {};
    RendererCore::ShaderProgram* m_LastShaderProgram = nullptr;
    std::stack<FrameBaker*> m_FrameBakers;
    Shader m_BaseShader;
};


GLIB_NAMESPACE_CLOSE