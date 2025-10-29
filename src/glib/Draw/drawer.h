#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"
#include "FontGenerator/font_generator.h"

#include <vector>
#include <stack>

#include "environment.h"
#include "Geometry/mesh.h"
#include "Geometry/text.h"
#include "structs.h"
#include "Utils/camera.h"
#include "texture.h"
#include "texture_manager.h"
#include "batch.h"
#include "shader.h"

GLIB_NAMESPACE_OPEN

constexpr float epsilon = 0.0005f;

class Drawer {
public:
    explicit Drawer(RendererCore::Window &window);

    void Start();
    void End();

    Camera* GetCamera();

    const glm::mat4& GetProjMatrix() const;

    // angle in degrees

    void DrawText(const Geom::Text2D& text2D, const Color& color, Shader* shader = nullptr);
    void DrawMesh(const Geom::Mesh& mesh, const Color& color, const Texture* texture = nullptr, Shader* shader = nullptr);
private:
    void InitDrawResources();
    void DrawBuffer();
    void Draw(const DrawResources& dr, const glm::mat4& mvp);
    void UseShader(Shader* shader);
private:
    RendererCore::Window *m_Window = nullptr;
    RendererCore::Renderer m_Renderer;

    DrawResources m_Gpu;

    TextureManager m_TexManager;
    const Texture* m_BasicTexture;
    std::shared_ptr<Shader> m_BasicShader;

    Batch m_Batch;

    Camera* m_Camera;
    glm::mat4 m_Proj  = glm::mat4(1.0f);

    static constexpr uint32_t MINIMUM_SIZE = 1;
};

GLIB_NAMESPACE_CLOSE