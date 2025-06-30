#include "glib.h"

namespace glib {

    Draw::Draw(GlCore::Window &window)
        : m_Window(&window)
    {
        InitDrawResources();
        m_Batch.BindDrawFunc([this]() {DrawBuffer();});
        m_TSlotManager.BindDrawFunc([this]() {DrawBuffer();});
        m_CreateShape = CreateShape(m_Window);

        m_Camera = Camera(&window);
        m_ShaderStack.push(m_Gpu.shader);
    }

    void Draw::InitDrawResources() {
        m_Gpu.shader = &GlCore::Cache.GetBasicProgram();
        m_Gpu.vertexArray = GlCore::VertexArray();
        m_Gpu.vertexBuffer = GlCore::VertexBuffer(GL_DYNAMIC_DRAW, 0, nullptr);
        m_Gpu.elementBuffer = GlCore::ElementBuffer(GL_DYNAMIC_DRAW, 0, nullptr);
        m_Gpu.basicTexture = GlCore::Texture();

        GlCore::VertexArrayLayout layout;
        layout.Add<float>(3);
        layout.Add<float>(4);
        layout.Add<float>(2);
        layout.Add<float>(1);
        m_Gpu.vertexArray.AddBuffer(layout, m_Gpu.vertexBuffer);

        m_Gpu.vertexArray.UnBind();
        m_Gpu.elementBuffer.UnBind();
        m_Gpu.vertexBuffer.UnBind();
    }

    Camera &Draw::GetCamera() {
        return m_Camera;
    }

    void Draw::Start() {
        m_Proj = glm::ortho(0.0f, (float) m_Window->GetWidth(),
                            0.0f, (float) m_Window->GetHeight(),
                            -100.0f, 100.0f);

        m_Renderer.Clear();
        m_Batch.BatchClear();
        m_TSlotManager.Clear();

        m_Camera.SetView(glm::mat4(1.0f));
    }

    void Draw::DrawBuffer() {
        m_TSlotManager.Bind();

        m_Gpu.vertexBuffer.PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
        m_Gpu.elementBuffer.PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

        glm::mat4 MVP = m_Proj * m_Camera.GetView();

        m_Gpu.shader->SetUniformMatrix4fv("u_MVP", &MVP[0][0]);
        m_Gpu.shader->SetUniform1iv("u_Texture", m_TSlotManager.GetMaxSlotsCount(), m_TSlotManager.GetSlotsData());
        m_Renderer.Draw(*m_Gpu.shader, m_Gpu.vertexArray, m_Gpu.elementBuffer);
    }

    void Draw::End() {
        DrawBuffer();

        m_Window->SwapDrawingBuffer();
    }

    void Draw::UseShader(Shader &shader) {
        DrawBuffer();

        m_Batch.BatchClear();
        m_TSlotManager.Clear();

        m_Gpu.shader = &shader.GetShader();
        m_ShaderStack.push(m_Gpu.shader);
        m_Gpu.shader->Bind();
    }

    void Draw::UnUseShader() {
        DrawBuffer();

        m_Batch.BatchClear();
        m_TSlotManager.Clear();

        m_ShaderStack.pop();
        m_Gpu.shader = m_ShaderStack.top();
        m_Gpu.shader->Bind();
    }

    void Draw::Rect(float x, float y, float width, float height, Color color) {
        int slot = m_TSlotManager.PushTexture(&m_Gpu.basicTexture);

        auto vertices = m_CreateShape.Rect(x, y, width, height, color, slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::Quad(float x, float y, float size, Color color) {
        Rect(x, y, size, size, color);
    }

    void Draw::Texture(float x, float y, float width, float height, const GlCore::Texture *texture) {
        int slot = m_TSlotManager.PushTexture(texture);

        auto vertices = m_CreateShape.RectTex(x, y, width, height, slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::Texture(const Rectangle &objProperties, const Rectangle &texProperties, const GlCore::Texture *texture) {
        int slot = m_TSlotManager.PushTexture(texture);

        auto vertices = m_CreateShape.RectTex(objProperties, texProperties, texture->GetWidth(), texture->GetHeight(), slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::QTexture(float x, float y, float size, const GlCore::Texture *texture) {
        Texture(x, y, size, size, texture);
    }
}