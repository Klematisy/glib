#include "drawer.h"
#include <numeric>

GLIB_NAMESPACE_OPEN

    Drawer::Drawer(RendererCore::Window &window)
            : m_Window(&window)
    {
        InitDrawResources();
        m_Batch.BindDrawFunc([this]() {DrawBuffer();});

        m_FBO = std::make_shared<FullscreenFBO>(window.GetWindow());

        m_BasicTexture = &m_TexManager.GetBasicTex();
        m_Gpu.shader = m_BasicShader->GetShaderProgram();
    }

    void Drawer::InitDrawResources() {
        m_BasicShader = std::make_shared<Shader>();
        m_BasicFontShader = std::make_shared<Shader>();

        m_BasicFontShader->AddSrcFiles("resources/shaders/font.glsl");
        m_BasicFontShader->Compile();

        m_BasicShader->AddSrcFiles("resources/shaders/base_shader.glsl");
        m_BasicShader->Compile();

        m_Gpu.vertexArray = std::make_shared<RendererCore::VertexArray>();
        m_Gpu.vertexBuffer = std::make_shared<RendererCore::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);
        m_Gpu.elementBuffer = std::make_shared<RendererCore::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);

        RendererCore::VertexArrayLayout layout;
        layout.Add<float>(3);
        layout.Add<float>(4);
        layout.Add<float>(3);
        m_Gpu.vertexArray->AddBuffer(layout, *m_Gpu.vertexBuffer);

        m_Gpu.vertexArray->UnBind();
        m_Gpu.vertexBuffer->UnBind();
        m_Gpu.elementBuffer->UnBind();
    }

    Camera* Drawer::GetCamera() {
        return m_Camera;
    }

    void Drawer::Start() {
        m_Proj = glm::ortho(0.0f, (float) m_Window->GetWidth(),
                            0.0f, (float) m_Window->GetHeight(),
                            -100.0f, 100.0f);

        m_Renderer.Clear();
        m_Batch.BatchClear();

        if (m_Camera) m_Camera->SetView(1.0f);
    }

    void Drawer::Draw(const DrawResources& dr, const glm::mat4& mvp) {
        dr.shader->Bind();

        dr.shader->SetUniformMatrix4fv("u_MVP", &mvp[0][0]);
        dr.shader->SetUniform1i("u_Texture", 0);
        m_Renderer.Draw(*dr.shader, *dr.vertexArray, *dr.elementBuffer);
    }

    void Drawer::DrawBuffer() {
        m_TexManager.Bind();

        m_Gpu.vertexBuffer->PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
        m_Gpu.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

        glm::mat4 MVP = m_Proj;
        if (m_Camera)  MVP *= m_Camera->GetView();

        Draw(m_Gpu, MVP);

        m_Batch.BatchClear();
    }

    void Drawer::End() {
        DrawBuffer();

        m_Window->SwapDrawingBuffer();
    }

    void Drawer::UseShader(Shader* shader) {
        Shader* sh;

        if (shader) {
            sh = shader;
        } else {
            sh = m_BasicShader.get();
        }

        if (sh->GetShaderProgram() == m_Gpu.shader) return;

        DrawBuffer();
        m_Batch.BatchClear();
        m_Gpu.shader = sh->GetShaderProgram();
    }

    void Drawer::DrawMesh(const Geom::Mesh &mesh, const Color& color, const Texture* texture, Shader* shader) {
        UseShader(shader);

        if (!texture) texture = m_BasicTexture;
        const TexInfo &tex = m_TexManager.GetTexInfo(texture);

        auto points = std::move(mesh.Bake());
        const auto& indices = mesh.GetIndices();
        const auto& uvs = mesh.GetUV();

        uint32_t size = points.size() / 3;
        std::vector<Vertex> vertices(size);

        for (uint32_t i = 0; i < size; i++) {
            uint32_t k = i * 3;
            glm::vec3 pos(points[k], (float) m_Window->GetHeight() - points[k + 1], 0.0f);

            uint32_t j = i * 2;
            const auto& t = *tex.GetTex();
            float uvX = ((float) tex.GetXOffset() + uvs[j]     * (float) t.GetWidth())  / TexInfo::WIDTH_MAX_SIZE;
            float uvY = ((float) tex.GetYOffset() + uvs[j + 1] * (float) t.GetHeight()) / TexInfo::HEIGHT_MAX_SIZE;

            glm::vec3 uv(uvX, uvY, tex.GetSlot());

            vertices[i] = {pos, color, uv};
        }

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Drawer::DrawText(const Geom::Text2D &text2D, const Color &color, Shader *shader) {
        UseShader(m_BasicFontShader.get());

        auto& txt = text2D.GetText();
        auto* font = text2D.GetFont();

        glm::vec3 position(50.0f, 50.0f, 0.0f);

        for (char c : txt) {
            auto info = font->GetGlyph(c, 40);

            const TexInfo &tex = m_TexManager.GetTexInfo(info.tex.get());
            Geom::Mesh mesh = Geom::MeshFactory::Get().CreateMesh("quad");
            float xOff = (float) tex.GetXOffset();
            float yOff = (float) tex.GetYOffset();

            int width = 0, height = 0, x = 0, y = 0;
            info.glyph->getBoxRect(x, y, width, height);
            mesh.SetScale({text2D.GetSize() * width, text2D.GetSize() * height, 1.0f});
            mesh.SetPosition(position);

            const auto& indices = mesh.GetIndices();
            auto points = std::move(mesh.Bake());

            mesh.SetUV({
                               (xOff + (float) x)          / TexInfo::WIDTH_MAX_SIZE, (yOff + (float) (y + height)) / TexInfo::HEIGHT_MAX_SIZE,
                               (xOff + (float) x)          / TexInfo::WIDTH_MAX_SIZE, (yOff + (float)  y)           / TexInfo::HEIGHT_MAX_SIZE,
                               (xOff + (float)(x + width)) / TexInfo::WIDTH_MAX_SIZE, (yOff + (float)  y)           / TexInfo::HEIGHT_MAX_SIZE,
                               (xOff + (float)(x + width)) / TexInfo::WIDTH_MAX_SIZE, (yOff + (float) (y + height)) / TexInfo::HEIGHT_MAX_SIZE,
                       });

            std::array<Vertex, 4> vertices;
            for (uint32_t i = 0; i < 4; i++) {
                uint32_t k = i * 3;
                uint32_t j = i * 2;

                glm::vec3 pos(points[k], (float) m_Window->GetHeight() - points[k + 1], points[k + 2]);
                glm::vec3 uv(mesh.GetUV()[j], mesh.GetUV()[j + 1], tex.GetSlot());

                vertices[i] = {pos, color, uv};
            }

            m_Batch.BatchVertices(vertices.data(), vertices.size());
            m_Batch.BatchIndices(indices.data(), indices.size());

//        position.x += info.glyph->getAdvance() * width * 5;
        }

        m_FBO->BeginCapture();

        m_Renderer.Clear();
        DrawBuffer();

        m_FBO->EndCapture();

        auto dr = m_FBO->GetDrawResources();
        dr.shader = m_BasicShader->GetShaderProgram();
        m_FBO->Bind();

        Draw(dr, glm::mat4(1.0f));
    }

    const glm::mat4& Drawer::GetProjMatrix() const {
        return m_Proj;
    }


GLIB_NAMESPACE_CLOSE