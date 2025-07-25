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

        m_FontStack.push(LangFontCache::GetCache().GetBasicFont());
    }

    void Draw::InitDrawResources() {
        m_Gpu.shader = &GlCore::ShaderCache::GetCache().GetBasicProgram();
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
        m_Model = 1.0f;
    }

    void Draw::DrawBuffer() {
        m_TSlotManager.Bind();

        m_Gpu.vertexBuffer.PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
        m_Gpu.elementBuffer.PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

        glm::mat4 MVP = m_Proj * m_Camera.GetView() * m_Model;

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

    void Draw::UseFont(Font &font) {
        m_FontStack.push(&font);
    }

    void Draw::UnUseFont() {
        m_FontStack.pop();
    }

    void Draw::Rect(const Rectangle &rect, float angleD, Color color) {
        int slot = m_TSlotManager.PushTexture(&m_Gpu.basicTexture);

        auto vertices = m_CreateShape.Rect(rect.x, rect.y, rect.width, rect.height, angleD, color, slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::Quad(const struct Quad &quad, float angleD, Color color) {
        Rect({quad.x, quad.y, quad.size, quad.size}, angleD, color);
    }

    void Draw::Texture(const Rectangle &rect, float angleD, const GlCore::Texture *texture) {
        int slot = m_TSlotManager.PushTexture(texture);

        auto vertices = m_CreateShape.RectTex(rect.x, rect.y, rect.width, rect.height, angleD, slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::Texture(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, const GlCore::Texture *texture) {
        int slot = m_TSlotManager.PushTexture(texture);

        auto vertices = m_CreateShape.RectTex(objProperties, texProperties, angleD, texture->GetWidth(), texture->GetHeight(), slot);
        auto  indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::QTexture(const struct Quad &quad, float angleD, const GlCore::Texture *texture) {
        Texture({quad.x, quad.y, quad.size, quad.size}, angleD, texture);
    }

    void Draw::Text(const std::wstring& text, struct Quad quad, float angleD, Color color) {
        float startX = quad.x;

        auto &tileSet = m_FontStack.top()->GetFontTileSet();
        if (quad.size < MINIMUM_SIZE) {
            quad.size = MINIMUM_SIZE;
        }

        quad.size *= 20;

        glm::vec2 midPoint = CreateShape::GetTextCenter(text, quad, tileSet);

        quad.y     = m_Window->GetHeight() - quad.y;
        midPoint.y = m_Window->GetHeight() - midPoint.y;

        for (wchar_t letter : text) {
            if (letter == L'\n') {
                quad.y -= quad.size;
                quad.x = startX;
                continue;
            }
            for (auto & langTile : tileSet) {
                if (langTile.GetFirstChar() <= letter && letter <= langTile.GetLastChar()) {
                    auto &tile = langTile.GetTile((uint32_t) quad.size);
                    int slot = m_TSlotManager.PushTexture(&tile.GetTexture());

                    auto letterVertices = m_CreateShape.Letter(&quad.x, &quad.y, midPoint, glm::radians(angleD), letter, tile, slot);
                    auto letterIndices  = CreateShape::RectangleIndices();

                    m_Batch.BatchVertices(letterVertices.data(), letterVertices.size());
                    m_Batch.BatchIndices(letterIndices.data(), letterIndices.size());

                    break;
                }
            }
        }
    }

    const glm::mat4 &Draw::GetProjMatrix() const {
        return m_Proj;
    }
}