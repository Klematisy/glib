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

void Geom::Text2D::SetText(const std::string& text) { m_Text = text; }
void Geom::Text2D::SetSize(uint32_t size)           { m_Size = size; }
void Geom::Text2D::SetFont(const Font* font)        { m_Font = font; }

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE