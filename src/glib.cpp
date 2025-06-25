#include "glib.h"

namespace glib {

    void Batch::BatchClear() {
        m_Vertices.clear();
        m_Indices.clear();
        m_MaxIndex = 0;
    }

    void Batch::BatchVertices(const Vertex* array, uint32_t size) {
        for (uint32_t i = 0; i < size; i++) {
            m_Vertices.push_back(array[i]);
        }
    }

    void Batch::BatchIndices(const uint32_t* array, uint32_t size) {
        uint32_t maxIndex = m_MaxIndex;
        for (uint32_t i = 0; i < size; i++) {
            m_Indices.push_back(array[i] + m_MaxIndex);
            maxIndex = std::max(array[i] + m_MaxIndex, maxIndex);
        }
        m_MaxIndex = ++maxIndex;
    }

    uint32_t Batch::GetVerticesSize() {
        return m_Vertices.size();
    }

    const void *Batch::GetVerticesData() {
        return m_Vertices.data();
    }

    uint32_t Batch::GetIndicesSize() {
        return m_Indices.size();
    }

    const void *Batch::GetIndicesData() {
        return m_Indices.data();
    }






    Draw::Draw(Window &window)
        : m_Window(&window)
    {

        m_Gpu.shader = ShaderProgram("resources/shaders/base_shader.glsl");
        m_Gpu.vertexArray = VertexArray();
        m_Gpu.vertexBuffer = VertexBuffer(GL_DYNAMIC_DRAW, 0, nullptr);
        m_Gpu.elementBuffer = ElementBuffer(GL_DYNAMIC_DRAW, 0, nullptr);
        m_Gpu.basicTexture = Texture();


        m_Gpu.vertexArray.Bind();
        VertexArrayLayout layout;
        layout.Add<float>(3);
        layout.Add<float>(4);
        layout.Add<float>(2);
        layout.Add<float>(1);
        m_Gpu.vertexArray.AddBuffer(layout, m_Gpu.vertexBuffer);

        m_Gpu.vertexArray.UnBind();
        m_Gpu.elementBuffer.UnBind();
        m_Gpu.vertexBuffer.UnBind();
    }

    void Draw::Start() {
        m_Proj = glm::ortho(0.0f, (float) m_Window->GetWidth(), 0.0f, (float) m_Window->GetHeight(), -1.0f, 1.0f);

        m_Renderer.Clear();
        m_Batch.BatchClear();
    }

    void Draw::End() {
        m_Gpu.basicTexture.Bind(0);
        m_Gpu.vertexBuffer.PutData(sizeof(Vertex) * m_Batch.GetVerticesSize(), m_Batch.GetVerticesData());
        m_Gpu.elementBuffer.PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

        m_Gpu.shader.SetUniformMatrix4fv("u_MVP", &m_Proj[0][0]);
        m_Gpu.shader.SetUniform1i("u_Texture", 0);
        m_Renderer.Draw(m_Gpu.shader, m_Gpu.vertexArray, m_Gpu.elementBuffer);

        m_Window->SwapDrawingBuffer();
    }

    void Draw::Rect(float x, float y, float width, float height, Color color) {
        auto vertices = CreateShape::Rectangle(x, y, width, height, color);
        auto indices = CreateShape::RectangleIndices();

        m_Batch.BatchVertices(vertices.data(), vertices.size());
        m_Batch.BatchIndices(indices.data(), indices.size());
    }

    void Draw::Quad(float x, float y, float size, Color color) {
        Rect(x, y, size, size, color);
    }




    std::array<Vertex, 4> CreateShape::Rectangle(float x, float y, float width, float height, Color color) {
        std::array<Vertex, 4> rect;

        rect[0] = {glm::vec3(x,         y,          0.0f), color};
        rect[1] = {glm::vec3(x,         y + height, 0.0f), color};
        rect[2] = {glm::vec3(x + width, y + height, 0.0f), color};
        rect[3] = {glm::vec3(x + width, y,          0.0f), color};

        return rect;
    }

    std::array<uint32_t, 6> CreateShape::RectangleIndices() {
        std::array<uint32_t, 6> indices {
            0, 1, 2,
            2, 3, 0
        };

        return indices;
    }
}