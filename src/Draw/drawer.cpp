#include <numeric>

#include "drawer.h"

GLIB_NAMESPACE_OPEN

static void initTexArrWithParam(std::shared_ptr<RendererCore::TextureArray>& texArr, GAPI::TEXTURE_PARAM texParam) {
    texArr->Bind();
    texArr->SetWidth(TexInfo::WIDTH_MAX_SIZE);
    texArr->SetHeight(TexInfo::HEIGHT_MAX_SIZE);
    texArr->SetLayersCount(16);

    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, texParam);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, texParam);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);

    texArr->AllocateTexture();
}


Drawer::Drawer(RendererCore::Window& window)
        : m_Window(&window)
{
    InitDrawResources();
    InitTextureArrays();
    m_BoundTexManager = &m_LinearTexManager;

    m_Batch.BindDrawFunc([this]() {DrawBuffer();});

    m_FontFramebuffer = std::make_shared<Framebuffer>(&window);
    m_FontFramebuffer->SetWidth(600);
    m_FontFramebuffer->SetHeight(600);

    m_Gpu.shader = m_BasicShader->GetShaderProgram();
}

void Drawer::InitTextureArrays() {
    m_BasicTexture = &m_NearestTexManager.GetBasicTex();

    std::shared_ptr<RendererCore::TextureArray> linearTextureArray = std::make_shared<RendererCore::TextureArray>();
    std::shared_ptr<RendererCore::TextureArray> nearestTextureArray = std::make_shared<RendererCore::TextureArray>();

    initTexArrWithParam(linearTextureArray, GAPI::TEXTURE_PARAM::LINEAR);
    initTexArrWithParam(nearestTextureArray, GAPI::TEXTURE_PARAM::NEAREST);

    m_LinearTexManager.SetTextureArray(linearTextureArray);
    m_NearestTexManager.SetTextureArray(nearestTextureArray);
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

void Drawer::Start() {
    m_Proj = glm::ortho(0.0f, (float) m_Window->GetWidth(),
                        0.0f, (float) m_Window->GetHeight(),
                        -100.0f, 100.0f);

    RendererCore::Renderer::Clear();
    m_Batch.BatchClear();

    if (m_Camera) m_Camera->SetView(1.0f);
}

void Drawer::Draw(const DrawResources& dr, const glm::mat4& mvp) {
    dr.shader->Bind();

    dr.shader->SetUniformMatrix4fv("u_MVP", &mvp[0][0]);
    dr.shader->SetUniform1i("u_Texture", 0);
    RendererCore::Renderer::Draw(*dr.shader, *dr.vertexArray, *dr.elementBuffer);
}

void Drawer::DrawBuffer() {
    if (m_Batch.GetVerticesSize() == 0 || m_Batch.GetIndicesSize() == 0) return;

    m_BoundTexManager->Bind();

    m_Gpu.vertexBuffer->PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
    m_Gpu.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    glm::mat4 MVP = m_Proj;
    if (m_Camera)  MVP *= m_Camera->GetView();

    Draw(m_Gpu, MVP);

    m_Batch.BatchClear();
}

void Drawer::RenderToFramebuffer(Framebuffer& fbo) {
    fbo.BeginCapture();
    RendererCore::Renderer::Clear();
    DrawBuffer();
    fbo.EndCapture();
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

    if (m_BoundTexManager == &m_LinearTexManager) {
        DrawBuffer();
    }

    m_BoundTexManager = &m_NearestTexManager;

    if (!texture) texture = m_BasicTexture;
    const TexInfo &tex = m_NearestTexManager.GetTexInfo(texture);

    auto& vertices = mesh.Bake();
    const auto& indices = mesh.GetIndices();
    const auto& uvs = mesh.GetUV();
    uint32_t windowHeight = m_Window->GetHeight();

    for (uint32_t i = 0; i < vertices.size(); i++) {

        uint32_t j = i * 2;
        const auto& t = *tex.GetTex();
        float uvX = ((float) tex.GetXOffset() + uvs[j]     * (float) t.GetWidth())  / TexInfo::WIDTH_MAX_SIZE;
        float uvY = ((float) tex.GetYOffset() + uvs[j + 1] * (float) t.GetHeight()) / TexInfo::HEIGHT_MAX_SIZE;

        glm::vec3 uv(uvX, uvY, tex.GetSlot());

        vertices[i].position.y = windowHeight - vertices[i].position.y;
        vertices[i].color = color;
        vertices[i].texCoords = uv;
    }

    m_Batch.BatchVertices(vertices.data(), vertices.size());
    m_Batch.BatchIndices(indices.data(), indices.size());
}

void Drawer::AddTextToBatch(const Geom::Text2D& text2D, const Color& color) {
    auto& txt = text2D.GetText();
    auto* font = text2D.GetFont();

    glm::vec3 position(text2D.ReadMesh()->GetPosition());

    uint32_t windowHeight = m_Window->GetHeight();

    for (char c : txt) {
        auto info = font->GetGlyph(c, 40);

        const TexInfo &tex = m_LinearTexManager.GetTexInfo(info.tex.get());
        Geom::Mesh mesh = Geom::MeshFactory::Get().CreateMesh("quad");
        float xOff = (float) tex.GetXOffset();
        float yOff = (float) tex.GetYOffset();

        int x = 0, y = 0, width = 0, height = 0;
        info.glyph->getBoxRect(x, y, width, height);
        mesh.SetScale({text2D.GetSize() * width, text2D.GetSize() * height, 1.0f});

        mesh.SetPosition({position.x, position.y, position.z});

        const auto& indices = mesh.GetIndices();
        auto& vertices = mesh.Bake();

        mesh.SetUV({
               (xOff + (float) x)          / TexInfo::WIDTH_MAX_SIZE, (yOff + (float) (y + height)) / TexInfo::HEIGHT_MAX_SIZE,
               (xOff + (float) x)          / TexInfo::WIDTH_MAX_SIZE, (yOff + (float)  y)           / TexInfo::HEIGHT_MAX_SIZE,
               (xOff + (float)(x + width)) / TexInfo::WIDTH_MAX_SIZE, (yOff + (float)  y)           / TexInfo::HEIGHT_MAX_SIZE,
               (xOff + (float)(x + width)) / TexInfo::WIDTH_MAX_SIZE, (yOff + (float) (y + height)) / TexInfo::HEIGHT_MAX_SIZE,
        });

        for (uint32_t i = 0; i < vertices.size(); i++) {
            uint32_t j = i * 2;

            glm::vec3 uv(mesh.GetUV()[j], mesh.GetUV()[j + 1], tex.GetSlot());

            vertices[i].position.y = (float) windowHeight - vertices[i].position.y;
            vertices[i].color = color;
            vertices[i].texCoords = uv;
        }

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());

        position.x += (float)(width + info.glyph->getAdvance()) * text2D.GetSize();
    }
}

void Drawer::DrawText(const Geom::Text2D &text2D, const Color &color, Shader* shader) {
    if (m_BoundTexManager == &m_NearestTexManager) {
        DrawBuffer();
    }
    m_BoundTexManager = &m_LinearTexManager;

    UseShader(m_BasicFontShader.get());
    AddTextToBatch(text2D, color);

    RenderToFramebuffer(*m_FontFramebuffer);

    auto dr = m_FontFramebuffer->GetDrawResources();
    if (shader) dr.shader = shader->GetShaderProgram();
    else dr.shader = m_BasicShader->GetShaderProgram();

    m_FontFramebuffer->Bind();
    Draw(dr, m_Proj);
}


GLIB_NAMESPACE_CLOSE