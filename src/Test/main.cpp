#include <memory>

#include "glm/glm.hpp"
#include "DrawUtils/batch.h"
#include "DrawUtils/shader.h"
#include "DrawUtils/texture_manager.h"

#include "Geometry/entity.h"
#include "Geometry/camera.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

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

static void initTexArrWithParam(RendererCore::ITexture* texArr, GAPI::TEXTURE_PARAM texParam) {
    GLIB_NAMESPACE_USING;

    RendererCore::TextureParameters tp = {
        texParam,
        texParam,
        GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE,
        GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE
    };

    texArr->SetTexParameters(tp);
}

GLIB_NAMESPACE_USING;

class EntityToVerticesEvaluator {
public:
    static std::vector<Vertex> Convert(const Geom::Entity& e) {
        glm::mat4 tm(1.0f);
        const auto& trans = *e.transition;
        glm::vec3 deltaPivot = trans.deltaPivot;

        tm = glm::translate(tm, trans.position);

        tm = glm::rotate(tm, glm::radians(trans.rotation.x), glm::vec3(1, 0, 0));
        tm = glm::rotate(tm, glm::radians(trans.rotation.y), glm::vec3(0, 1, 0));
        tm = glm::rotate(tm, glm::radians(trans.rotation.z), glm::vec3(0, 0, 1));

        tm = glm::scale(tm, trans.scale);
        tm = glm::translate(tm, -deltaPivot);

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

        return vertices;
    }
};


int main() {
    RendererCore::Window window(600, 600, "glib");

    auto basicGB = CreateDrawBasicsResources();

    std::shared_ptr<RendererCore::ITexture> textureArray = std::make_shared<RendererCore::TextureArray>(3000, 3000, 16);
    initTexArrWithParam(textureArray.get(), GAPI::TEXTURE_PARAM::NEAREST);
    TextureManager textureManager(textureArray);

    RendererCore::ImageInfo bmb("resources/images/beautiful_minimalistic_boy.png");
    RendererCore::ImageInfo cat("resources/images/cat.png");
    RendererCore::ImageInfo wonam("resources/images/wonam.jpg");
    RendererCore::ImageInfo grass_block("resources/images/grass_block.png");
    RendererCore::ImageInfo gayBlock("resources/images/gayBlock.png");

    std::shared_ptr<uint8_t> bitmap(new uint8_t[4], [](const uint8_t* p) { delete[] p; });
    bitmap.get()[0] = 255;
    bitmap.get()[1] = 255;
    bitmap.get()[2] = 255;
    bitmap.get()[3] = 255;

    RendererCore::ImageInfo tex(1, 1, 4, bitmap);

    textureManager.GetTexInfo(&tex);
    auto texInfo = textureManager.GetTexInfo(&wonam);
    textureManager.GetTexInfo(&bmb);
    textureManager.GetTexInfo(&gayBlock);
    textureManager.GetTexInfo(&grass_block);

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    RendererCore::Renderer renderer;
    renderer.SetRendererType(GAPI::RENDERER_TYPE::TRIANGLES);

    Shader shader;
    shader.AddSrcFiles("resources/shaders/base_shader.glsl");
    shader.Compile();

    Batch<Vertex> batch;
    batch.SetMaxBatchSize(10'000);

    Geom::Entity e;

    e.mesh = std::make_shared<Geom::Mesh>();
    e.material = std::make_shared<Geom::Material>();
    e.transition = std::make_shared<Geom::Transform>();

    *e.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    e.material->shader = shader.GetShaderProgram().get();
    e.material->uvCoordinates = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
    };

    e.transition->deltaPivot.x = 0.5f;
    e.transition->deltaPivot.y = 0.5f;
    e.transition->position.x = 1.0f;
    e.transition->position.y = 1.0f;

    e.transition->scale = {1.0, 1.0, 1.0};


    auto vertices = EntityToVerticesEvaluator::Convert(e);

    for (auto& it : vertices) {
        it.uv.x *= texInfo->GetRectangle().width;
        it.uv.x += texInfo->GetRectangle().x;

        it.uv.y *= texInfo->GetRectangle().height;
        it.uv.y += texInfo->GetRectangle().y;

        it.uv.z = texInfo->GetSlot();
    }

    batch.AddVertices(vertices.data(), vertices.size());
    batch.AddIndices(e.mesh->indices.data(), e.mesh->indices.size());

    basicGB.vertexBuffer->PutData(batch.GetVerticesSize() * sizeof(Vertex), batch.GetVerticesData());
    basicGB.elementBuffer->PutData(batch.GetIndicesSize(), batch.GetIndicesData());

    batch.Clear();


    OrthographicCamera cam(&window);
    cam.SetRenderRange(0.0f, 2.0f, 0.0f, 2.0f);

    while (window.IsOpen()) {
        renderer.Clear();

        e.material->shader->Bind();
        e.material->shader->SetUniform1i("u_Texture", 0);
        e.material->shader->SetUniformMatrix4fv("u_MVP", &cam.GetVP()[0][0]);

        textureManager.Bind();
        renderer.Draw(basicGB, *e.material->shader);

        window.SwapDrawingBuffer();
    }

    return 0;
}