#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"
#include "FontGenerator/font_generator.h"

#include <vector>
#include <stack>

#include "Geometry/mesh.h"
#include "Geometry/text.h"
#include "Utils/camera.h"
#include "Utils/frame_buffer.h"
#include "environment.h"
#include "structs.h"
#include "texture.h"
#include "texture_manager.h"
#include "batch.h"
#include "shader.h"

GLIB_NAMESPACE_OPEN

class Drawer {
public:
    explicit Drawer(RendererCore::Window &window);

    void Start();
    void End();

    void DrawText(const Geom::Text2D& text2D, const Color& color, Shader* shader = nullptr);
    void DrawMesh(const Geom::Mesh& mesh, const Color& color, const Texture* texture = nullptr, Shader* shader = nullptr);

private:
    void InitDrawResources();
    void InitTextureArrays();
    void DrawBuffer();
    void UseShader(Shader* shader);
    void UseTextureManager(const TextureManager& textureManager);
private:
    RendererCore::Window* m_Window = nullptr;
    DrawResources m_Gpu;
    Batch m_Batch;

    TextureManager m_LinearTexManager;
    TextureManager m_NearestTexManager;
    const TextureManager* m_BoundTexManager = nullptr;

    const Texture* m_BasicTexture;
    std::shared_ptr<Shader> m_BasicShader;

    Camera* m_Camera;
    glm::mat4 m_Proj  = glm::mat4(1.0f);
};

GLIB_NAMESPACE_CLOSE