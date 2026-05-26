#pragma once

#include "Graphics/RendererCore/renderer.h"
#include "Geometry/camera.h"
#include "Geometry/entity.h"

#include "texture_manager.h"
#include "frame_buffer.h"
#include "shader.h"
#include "batch.h"
#include "structs.h"

VLADLIB_NAMESPACE_OPEN

class RenderCaller {
public:
    RenderCaller();

    void Call(const Batch<Vertex>& batch, const RendererCore::ShaderProgram* shader, const RendererCore::ITexture* texture);
private:
    RendererCore::Renderer m_Renderer;
    RendererCore::RenderItem m_Item;
};

class EntityToVerticesEvaluator {
public:
    static std::vector<Vertex> Convert(const Geom::Entity& e, const TexInfoConstRef& texInfo);
};

class TransformConfirmer {
public:
    static Geom::Mesh ConfirmMesh(const Geom::Mesh& e, const Geom::Transform& t);
    static std::vector<glm::vec3> ConfirmPoints(std::vector<glm::vec3> e, const Geom::Transform& t);
};

class SceneRenderer {
public:
    SceneRenderer(RendererCore::Window* window);

    void StartDraw();
    void EndDraw();
    void DrawEntity(const Geom::Entity& e);

    void UseCamera(Camera* cam);
    Camera* GetCamera() const;

    void RegisterFrameBaker(const FrameBaker& fm);
    void StartBake(FrameBaker& fm);
    void EndBake();
private:
    void FlushBatch();
private:
    RendererCore::ImageInfo m_StandardTex;
    Shader m_BaseShader;

    TextureManager m_TexManager {3000, 3000};
    Batch<Vertex> m_Batch;

    Camera* m_Camera = nullptr;
    RendererCore::Window* m_Window = nullptr;

    RendererCore::RenderItem m_Item;
    RendererCore::Renderer m_Renderer;
    std::stack<std::pair<FrameBaker*, RendererCore::Rectanglei>> m_FrameBakers;
};


VLADLIB_NAMESPACE_CLOSE
