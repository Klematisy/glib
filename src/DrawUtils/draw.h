#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "Graphics/RendererCore/renderer.h"
#include "Geometry/camera.h"
#include "shader.h"
#include "Geometry/entity.h"
#include "texture_manager.h"
#include "batch.h"

GLIB_NAMESPACE_OPEN

struct Vertex {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec3 uv = glm::vec3(1.0f);
};

class EntityToVerticesEvaluator {
public:
    static std::vector<Vertex> Convert(const Geom::Entity& e, const TexInfoConstRef& texInfo) {
        glm::mat4 tm(1.0f);
        if (e.transform) {
            const auto& trans = *e.transform;
            glm::vec3 deltaPivot = trans.deltaPivot;

            tm = glm::translate(tm, trans.position);

            tm = glm::rotate(tm, glm::radians(trans.rotation.x), glm::vec3(1, 0, 0));
            tm = glm::rotate(tm, glm::radians(trans.rotation.y), glm::vec3(0, 1, 0));
            tm = glm::rotate(tm, glm::radians(trans.rotation.z), glm::vec3(0, 0, 1));

            tm = glm::scale(tm, trans.scale);
            tm = glm::translate(tm, -deltaPivot);
        }

        const auto& p = e.mesh->points;
        const auto& uv = e.material->uvCoordinates;

        static std::vector<Vertex> vertices;
        vertices.clear();

        for (uint32_t i = 0; i < p.size(); i++) {
            glm::vec4 point = {p[i], 1.0f};
            glm::vec3 uvCord = {
                    uv[i % uv.size()].x,
                    uv[i % uv.size()].y,
                    0
            };
            vertices.push_back({.pos = tm * point, .uv = uvCord});
        }

        for (auto& it : vertices) {
            it.uv.x *= texInfo->GetRectangle().width;
            it.uv.x += texInfo->GetRectangle().x;

            it.uv.y *= texInfo->GetRectangle().height;
            it.uv.y += texInfo->GetRectangle().y;

            it.uv.z = texInfo->GetSlot();
        }

        return vertices;
    }
};



class Draw {
public:
    Draw(RendererCore::Window* window);

    void StartDraw();
    void EndDraw();
    void DrawEntity(const Geom::Entity& e);

    void UseCamera(Camera* cam);
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
    Shader m_BaseShader;
};


GLIB_NAMESPACE_CLOSE