#include "text.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

Geom::Text2D::Text2D(const std::string& str, const Font* font, uint32_t size)
    : m_Text(str), m_Font(font), m_Size(size),
    m_Mesh(std::make_shared<Mesh>(Geom::MeshFactory::Get().CreateMesh("quad")))
{}

const std::string &Geom::Text2D::GetText() const        { return m_Text; }
uint32_t Geom::Text2D::GetSize() const                  { return m_Size; }
const Font* Geom::Text2D::GetFont() const               { return m_Font; }

std::shared_ptr<Mesh> Geom::Text2D::ReadMesh() const    { return m_Mesh; }
std::shared_ptr<Mesh> Geom::Text2D::WriteMesh()         { return m_Mesh; }

void Geom::Text2D::SetText(const std::string& text) {
    m_Text = text;
    m_Dirty = true;
}

void Geom::Text2D::SetSize(uint32_t size) {
    m_Size = size;
    m_Dirty = true;
}

void Geom::Text2D::SetFont(const Font* font) {
    m_Font = font;
    m_Dirty = true;
}

const glm::vec3& Text2D::GetTextScreenSize() const {
    if (!m_Dirty)
        return m_TextScreenSize;
    m_Dirty = false;

    m_TextScreenSize.x = 0.0f;
    m_TextScreenSize.y = 90 * m_Size;

    for (char c : m_Text) {
        if (c == '\n') {
            m_TextScreenSize.y += 90 * m_Size;
            continue;
        }

        auto info = m_Font->GetGlyph(c, 40);

        int x = 0, y = 0, width = 0, height = 0;
        info.glyph->getBoxRect(x, y, width, height);

        m_TextScreenSize.x += (float)(width + info.glyph->getAdvance()) * (float) m_Size;
    }

    return m_TextScreenSize;
}

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE