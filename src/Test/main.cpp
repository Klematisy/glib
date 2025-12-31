#include <memory>

#include "glm/glm.hpp"
#include "Draw/batch.h"
#include "Draw/shader.h"
#include "Draw/texture_manager.h"

#include "Geometry/entity.h"

#include "Utils/camera.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

static RendererCore::Window window(600, 600, "glib");

static RendererCore::GraphicsBuffer CreateDrawBasicsResources() {
    RendererCore::GraphicsBuffer dr;

    dr.vertexArray = std::make_shared<RendererCore::VertexArray>();
    dr.vertexBuffer = std::make_shared<RendererCore::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);
    dr.elementBuffer = std::make_shared<RendererCore::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);

    RendererCore::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);
    dr.vertexArray->AddBuffer(layout, *dr.vertexBuffer);

    dr.vertexArray->UnBind();
    dr.vertexBuffer->UnBind();
    dr.elementBuffer->UnBind();

    return dr;
}

struct Vertex {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec3 uv = glm::vec3(1.0f);
};

static void initTexArrWithParam(RendererCore::TextureArray& texArr, GAPI::TEXTURE_PARAM texParam) {
    GLIB_NAMESPACE_USING;

    texArr.Bind();
    texArr.SetWidth(TexArrElInfo::WIDTH_MAX_SIZE);
    texArr.SetHeight(TexArrElInfo::HEIGHT_MAX_SIZE);

    texArr.SetLayersCount(16);

    texArr.Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, texParam);
    texArr.Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, texParam);
    texArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    texArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);

    texArr.AllocateTexture();
}

int main() {
    GLIB_NAMESPACE_USING;

    auto basicGB = CreateDrawBasicsResources();

    TextureManager textureManager;
    auto textureArray = std::make_shared<RendererCore::TextureArray>();
    initTexArrWithParam(*textureArray, GAPI::TEXTURE_PARAM::NEAREST);
    textureManager.SetTextureArray(textureArray);

    auto tex = TextureManager::GetBasicTex();
    auto texInfo = textureManager.GetTexInfo(&tex);

    RendererCore::Renderer renderer;
    renderer.SetRendererType(GAPI::RENDERER_TYPE::TRIANGLES);

    Batch<Vertex> batch;
    batch.SetMaxBatchSize(10'000);

    Shader shader;
    shader.AddSrcFiles("resources/shaders/base_shader.glsl");
    shader.Compile();


    Geom::Entity e;

    e.mesh = std::make_shared<Geom::Mesh>();
    *e.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    e.material = std::make_shared<Geom::Material>();
    e.material->shader = shader.GetShaderProgram().get();
    e.material->uvCoordinates = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };

    auto& p = e.mesh->points;
    auto& uv = e.material->uvCoordinates;

    std::array<Vertex, 4> quad_verts;
    quad_verts[0] = {.pos = {p[0], p[1],  p[2] },   .uv = {uv[0] / TexArrElInfo::WIDTH_MAX_SIZE, uv[1] / TexArrElInfo::HEIGHT_MAX_SIZE, texInfo.GetSlot()}};
    quad_verts[1] = {.pos = {p[3], p[4],  p[5] },   .uv = {uv[2] / TexArrElInfo::WIDTH_MAX_SIZE, uv[3] / TexArrElInfo::HEIGHT_MAX_SIZE, texInfo.GetSlot()}};
    quad_verts[2] = {.pos = {p[6], p[7],  p[8] },   .uv = {uv[4] / TexArrElInfo::WIDTH_MAX_SIZE, uv[5] / TexArrElInfo::HEIGHT_MAX_SIZE, texInfo.GetSlot()}};
    quad_verts[3] = {.pos = {p[9], p[10], p[11]},   .uv = {uv[6] / TexArrElInfo::WIDTH_MAX_SIZE, uv[7] / TexArrElInfo::HEIGHT_MAX_SIZE, texInfo.GetSlot()}};

    batch.AddVertices(quad_verts.cbegin(), 4);
    batch.AddIndices(e.mesh->indices.data(), e.mesh->indices.size());

    OrthographicCamera cam(&window);
    cam.SetRenderRange(0.0f, 2.0f, 0.0f, 2.0f);

    basicGB.vertexBuffer->PutData(batch.GetVerticesSize() * sizeof(Vertex), batch.GetVerticesData());
    basicGB.elementBuffer->PutData(batch.GetIndicesSize(), batch.GetIndicesData());

    while (window.IsOpen()) {
        renderer.Clear();

        textureManager.Bind();
        e.material->shader->Bind();
        e.material->shader->SetUniform1i("u_Texture", 0);
        e.material->shader->SetUniformMatrix4fv("u_MVP", &cam.GetVP()[0][0]);
        renderer.Draw(basicGB, *shader.GetShaderProgram());

        window.SwapDrawingBuffer();
    }

    return 0;
}
