#pragma once

#include <memory>
#include <stack>

#include "Geometry/camera.h"
#include "Geometry/entity.h"

#include "GraphicsAPI/graphics_api.h"
#include "GraphicsAPI/window.h"
#include "texture_manager.h"
#include "frame_buffer.h"
#include "shader.h"
#include "batch.h"
#include "structs.h"

#include "GraphicsAPI/common.h"

VLADLIB_NAMESPACE_OPEN

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
    SceneRenderer(GAPI::WindowPTR window);

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
    GAPI::ImageInfo m_StandardTex;
    Shader m_BaseShader;

    TextureManager m_TexManager {3000, 3000};
    Batch<Vertex> m_Batch;

    Camera* m_Camera = nullptr;
    GAPI::WindowPTR m_Window = nullptr;

    GAPI::RenderItem m_Item;
    std::shared_ptr<GAPI::Renderer> m_Renderer = GAPI::createRenderer();
    std::stack<std::pair<FrameBaker*, Rectanglei>> m_FrameBakers;
};


VLADLIB_NAMESPACE_CLOSE
