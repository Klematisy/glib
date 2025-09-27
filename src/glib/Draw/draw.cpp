#include "draw.h"

GLIB_NAMESPACE_OPEN

Draw::Draw(GlCore::Window &window)
    : m_Window(&window)
{
    InitDrawResources();
    m_Batch.BindDrawFunc([this]() {DrawBuffer();});
    m_CreateShape = CreateShape(m_Window);

    m_BasicTexture = &m_TexManager.GetBasicTex();
    m_Gpu.shader = m_BasicProgram;

    m_FontStack.push(LangFontCache::GetCache().GetBasicFont());
}

void Draw::InitDrawResources() {
    m_BasicProgram = &GlCore::ShaderCache::GetCache().GetBasicProgram();

    m_Gpu.vertexArray = GlCore::VertexArray();
    m_Gpu.vertexBuffer = GlCore::VertexBuffer(GL_DYNAMIC_DRAW, 0, nullptr);
    m_Gpu.elementBuffer = GlCore::ElementBuffer(GL_DYNAMIC_DRAW, 0, nullptr);

    GlCore::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);
    m_Gpu.vertexArray.AddBuffer(layout, m_Gpu.vertexBuffer);

    m_Gpu.vertexArray.UnBind();
    m_Gpu.vertexBuffer.UnBind();
    m_Gpu.elementBuffer.UnBind();
}

Camera* Draw::GetCamera() {
    return m_Camera;
}

void Draw::Start() {
    m_Proj = glm::ortho(0.0f, (float) m_Window->GetWidth(),
                        0.0f, (float) m_Window->GetHeight(),
                        -100.0f, 100.0f);

    m_Renderer.Clear();
    m_Batch.BatchClear();

    if (m_Camera) m_Camera->SetView(1.0f);
}

void Draw::DrawBuffer() {
    m_TexManager.Bind();

    m_Gpu.vertexBuffer.PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
    m_Gpu.elementBuffer.PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    glm::mat4 MVP = m_Proj;
    if (m_Camera)  MVP *= m_Camera->GetView();

    m_Gpu.shader->SetUniformMatrix4fv("u_MVP", &MVP[0][0]);
    m_Gpu.shader->SetUniform1i("u_Texture", 0);
    m_Renderer.Draw(*m_Gpu.shader, m_Gpu.vertexArray, m_Gpu.elementBuffer);
}

void Draw::End() {
    DrawBuffer();

    m_Window->SwapDrawingBuffer();
}

void Draw::UseShader(GlCore::ShaderProgram* shader) {
    if (shader) {
        DrawBuffer();
        m_Batch.BatchClear();
        m_Gpu.shader = shader;
        m_Gpu.shader->Bind();
    }
}

void Draw::UseFont(Font &font) {
    m_FontStack.push(&font);
}

void Draw::UnUseFont() {
    m_FontStack.pop();
}

void Draw::DrawMesh(const Geom::Mesh &mesh, const Color& color, const Texture *texture, Shader *shader) {
    if (!texture) texture = m_BasicTexture;

    if (!shader) UseShader(m_BasicProgram);
    else if (&shader->GetShader() != m_Gpu.shader) UseShader(&shader->GetShader());

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

void Draw::Text(const std::wstring& text, struct Quad quad, float angleD, Color color) {
    float startX = quad.x;

    auto &tileSet = m_FontStack.top()->GetFontTileSet();
    if (quad.size < MINIMUM_SIZE) {
        quad.size = MINIMUM_SIZE;
    }

    quad.size *= 20;

    glm::vec2 midPoint = CreateShape::GetTextCenter(text, quad, tileSet);

    quad.y = m_Window->GetHeight() - quad.y;
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
                const TexInfo &tex = m_TexManager.GetTexInfo(tile.GetTexture());

                auto letterVertices = m_CreateShape.Letter(&quad.x, &quad.y, midPoint, glm::radians(angleD), letter, tile, tex);
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

GLIB_NAMESPACE_CLOSE