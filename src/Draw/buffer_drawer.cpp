#include <numeric>

#include "buffer_drawer.h"

GLIB_NAMESPACE_OPEN

void BufferDrawer::Start() {
    RendererCore::Renderer::Clear();
}

void BufferDrawer::End() {
    FlushBuffer();
}

void BufferDrawer::FlushBuffer() {
    if (m_Batch.GetVerticesSize() == 0 || m_Batch.GetIndicesSize() == 0) return;
    assert(m_BoundDrawBuffer);

    m_BoundTexManager->Bind();
    auto& dr = *m_BoundDrawBuffer;

    dr.vertexBuffer->PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
    dr.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    glm::mat4 mvp = m_Camera->GetVP();
    auto boundShaderProgram = m_BoundShader->GetShaderProgram();
    boundShaderProgram->Bind();

    boundShaderProgram->SetUniformMatrix4fv("u_MVP", &mvp[0][0]);
    boundShaderProgram->SetUniform1i("u_Texture", 0);
    RendererCore::Renderer::Draw(*boundShaderProgram, *dr.vertexArray, *dr.elementBuffer);

    m_Batch.Clear();
}

//void BufferDrawer::NormalizePoint(glm::vec3& p) {
//    float l = m_RenderRange[0];
//    float r = m_RenderRange[1];
//    float b = m_RenderRange[2];
//    float t = m_RenderRange[3];
//
//    float far = -600.0f;
//    float near = 600.0f;
//
//    float mid_x = std::fabsf(l - r) / 2;
//    float mid_x_point = std::fmaxf(l, r) - mid_x;
//    p.x = (p.x - mid_x_point) / mid_x;
//    p.x *= (l > r) ? -1 : 1;
//
//    float mid_y = std::fabsf(b - t) / 2;
//    float mid_y_point = std::fmaxf(b, t) - mid_y;
//    p.y = (p.y - mid_y_point) / mid_y;
//    p.y *= (b > t) ? -1 : 1;
//
//    float mid_z = std::fabsf(far - near) / 2;
//    float mid_z_point = std::fmaxf(far, near) - mid_z;
//    p.z = (p.z - mid_z_point) / mid_z;
//    p.z *= (far > near) ? 1 : -1;
//}

static std::vector<Vertex> vertices;

void BufferDrawer::BatchMesh(const Geom::Mesh& mesh, const Texture* texture) {
    const TexInfo& tex = m_BoundTexManager->GetTexInfo(texture);

    const auto& uvs = mesh.GetUV();
    const auto& indices = mesh.GetIndices();

    vertices.clear();
    vertices = Geom::MeshBaker::Bake(mesh);

    uint32_t w = m_BoundTexManager->GetTexArray()->GetWidth();
    uint32_t h = m_BoundTexManager->GetTexArray()->GetHeight();

    for (uint32_t i = 0; i < vertices.size(); i++) {
        uint32_t j = (i * 2) % uvs.size();
        const auto& t = *tex.GetTex();

        vertices[i].texCoords.x = ((float) tex.GetXOffset() + uvs[j]     * (float) t.GetWidth())  / w;
        vertices[i].texCoords.y = ((float) tex.GetYOffset() + uvs[j + 1] * (float) t.GetHeight()) / h;
        vertices[i].texCoords.z = tex.GetSlot();
    }

    m_Batch.AddVertices(vertices.data(), vertices.size());
    m_Batch.AddIndices(indices.data(), indices.size());
}

void BufferDrawer::BatchText(const Geom::Text2D& text2D) {
    auto& txt = text2D.GetText();
    auto* font = text2D.GetFont();

    glm::vec3 position(0.0f);

    uint32_t w = m_BoundTexManager->GetTexArray()->GetWidth();
    uint32_t h = m_BoundTexManager->GetTexArray()->GetHeight();

    for (char c : txt) {
        auto info = font->GetGlyph(c, 40);

        const TexInfo& tex = m_BoundTexManager->GetTexInfo(info.tex.get());
        Geom::Mesh mesh = Geom::MeshFactory::Get().CreateMesh("quad");
        float xOff = (float) tex.GetXOffset();
        float yOff = (float) tex.GetYOffset();

        int x = 0, y = 0, width = 0, height = 0;
        info.glyph->getBoxRect(x, y, width, height);
        mesh.SetScale({text2D.GetSize() * width, text2D.GetSize() * height, 1.0f});
        mesh.SetPosition({position.x, position.y, position.z});

        const auto& indices = mesh.GetIndices();
        vertices.clear();
        vertices = Geom::MeshBaker::Bake(mesh);

        mesh.SetUV({
               (xOff + (float) x)          / w, (yOff + (float) (y + height)) / h,
               (xOff + (float) x)          / w, (yOff + (float)  y)           / h,
               (xOff + (float)(x + width)) / w, (yOff + (float)  y)           / h,
               (xOff + (float)(x + width)) / w, (yOff + (float) (y + height)) / h,
        });

        for (uint32_t i = 0; i < vertices.size(); i++) {
            uint32_t j = i * 2;

            glm::vec3 uv(mesh.GetUV()[j], mesh.GetUV()[j + 1], tex.GetSlot());
            vertices[i].texCoords = uv;
        }

        m_Batch.AddVertices(vertices.data(), vertices.size());
        m_Batch.AddIndices(indices.data(), indices.size());

        position.x += (float)(width + info.glyph->getAdvance()) * text2D.GetSize();
    }
}

void BufferDrawer::UseCamera(Camera* camera)                         { m_Camera = camera;                  }
void BufferDrawer::UseShader(Shader* shader)                         { m_BoundShader = shader;             }
void BufferDrawer::UseBuffer(DrawBuffer* drawResources)              { m_BoundDrawBuffer = drawResources;  }
void BufferDrawer::UseTextureManager(TextureManager* textureManager) { m_BoundTexManager = textureManager; }

const TextureManager* BufferDrawer::GetBoundTexManager() const { return m_BoundTexManager; }
const DrawBuffer* BufferDrawer::GetDrawBuffer() const          { return m_BoundDrawBuffer; }
const Shader* BufferDrawer::GetBoundShader() const             { return m_BoundShader;     }


GLIB_NAMESPACE_CLOSE

