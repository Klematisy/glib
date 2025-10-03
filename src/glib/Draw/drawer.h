#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "OpenGLCore/renderer.h"
#include "OpenGLCore/window.h"
#include "FontGenerator/font.h"

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
    explicit Drawer(GlCore::Window &window);

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
    void UseShader(GlCore::ShaderProgram* shader);
private:
    GlCore::Window *m_Window = nullptr;
    GlCore::Renderer m_Renderer;

    DrawResources m_Gpu;

    TextureManager m_TexManager;
    const Texture* m_BasicTexture;
    GlCore::ShaderProgram* m_BasicProgram;

    Batch m_Batch;

    Camera* m_Camera;
    glm::mat4 m_Proj  = glm::mat4(1.0f);

    static constexpr uint32_t MINIMUM_SIZE = 1;
};

GLIB_NAMESPACE_CLOSE