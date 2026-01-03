#include <memory>

#include "glm/glm.hpp"
#include "Draw/batch.h"
#include "Draw/shader.h"
#include "Draw/texture_manager.h"

#include "Geometry/entity.h"

#include "Utils/camera.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

static glm::vec3 rotate_about_vec(const glm::vec3& src,
                                  const glm::vec3& axis,
                                  float angle) {
    float cos = std::cosf(angle);
    float sin = std::sinf(angle);
    return src * cos + glm::cross(axis, src) * sin + axis * glm::dot(axis, src) * (1 - cos);
}

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

    RendererCore::Window window(600, 600, "glib");

    auto basicGB = CreateDrawBasicsResources();

    TextureManager textureManager;
    auto textureArray = std::make_shared<RendererCore::TextureArray>();
    initTexArrWithParam(*textureArray, GAPI::TEXTURE_PARAM::NEAREST);
    textureManager.SetTextureArray(textureArray);

    Texture cat("resources/images/beautiful_minimalistic_boy.png");
    auto texInfo = textureManager.GetTexInfo(&cat);

    RendererCore::Renderer renderer;
    renderer.SetRendererType(GAPI::RENDERER_TYPE::TRIANGLES);

    Batch<Vertex> batch;
    batch.SetMaxBatchSize(10'000);

    Shader shader;
    shader.AddSrcFiles("resources/shaders/base_shader.glsl");
    shader.Compile();

    Geom::Entity e;

    e.mesh = std::make_shared<Geom::Mesh>();
    e.material = std::make_shared<Geom::Material>();
    e.transition = std::make_shared<Geom::Transition>();

    *e.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    e.material->shader = shader.GetShaderProgram().get();
    e.material->uvCoordinates = {
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
    };

    e.transition->rotation.z = 0.f;
    e.transition->deltaPivot.x = 0.5f;
    e.transition->deltaPivot.y = 0.5f;
    e.transition->position.x = 1.0f;
    e.transition->position.y = 1.0f;


    glm::mat4 tm(1.0f);
    const auto& trans = *e.transition;
    glm::vec3 deltaPivot = trans.deltaPivot;
    Geom::Basis basis;

    tm = glm::translate(tm, trans.position);

    tm = glm::rotate(tm, glm::radians(trans.rotation.x), basis.xAxis);
    basis.yAxis = rotate_about_vec(basis.yAxis, basis.xAxis, -glm::radians(trans.rotation.x));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.xAxis, -glm::radians(trans.rotation.x));

    tm = glm::rotate(tm, glm::radians(trans.rotation.y), basis.yAxis);
    basis.xAxis = rotate_about_vec(basis.xAxis, basis.yAxis, -glm::radians(trans.rotation.y));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.yAxis, -glm::radians(trans.rotation.y));

    tm = glm::rotate(tm, glm::radians(trans.rotation.z), basis.zAxis);

    tm = glm::scale(tm, trans.scale);

    tm = glm::translate(tm, -deltaPivot);

    const auto& p = e.mesh->points;
    const auto& uv = e.material->uvCoordinates;

    for (uint32_t i = 0; i < p.size(); i++) {
        glm::vec4 point = {p[i], 1.0f};
        glm::vec3 uvCord = {
                (uv[i].x * (float) texInfo.GetTex()->GetWidth()  + (float) texInfo.GetXOffset()) / TexArrElInfo::WIDTH_MAX_SIZE,
                (uv[i].y * (float) texInfo.GetTex()->GetHeight() + (float) texInfo.GetYOffset()) / TexArrElInfo::HEIGHT_MAX_SIZE,
                texInfo.GetSlot()
        };
        Vertex vert = {.pos = tm * point, .uv = uvCord};
        batch.AddVertices(&vert, 1);
    }
    batch.AddIndices(e.mesh->indices.data(), e.mesh->indices.size());

    basicGB.vertexBuffer->PutData(batch.GetVerticesSize() * sizeof(Vertex), batch.GetVerticesData());
    basicGB.elementBuffer->PutData(batch.GetIndicesSize(), batch.GetIndicesData());

    batch.Clear();


    OrthographicCamera cam(&window);
    cam.SetRenderRange(0.0f, 2.0f, 2.0f, 0.0f);

    while (window.IsOpen()) {
        renderer.Clear();

        textureManager.Bind();
        e.material->shader->Bind();
        e.material->shader->SetUniform1i("u_Texture", 0);
        e.material->shader->SetUniformMatrix4fv("u_MVP", &cam.GetVP()[0][0]);
        renderer.Draw(basicGB, *e.material->shader);

        window.SwapDrawingBuffer();
    }

    return 0;
}
