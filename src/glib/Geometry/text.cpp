#include "text.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

glib::Geom::Text2D::Text2D(const std::string &str, const glib::Font *font, uint32_t size)
    : m_Text(str), m_Font(font), m_Size(size)
{}

const std::string &glib::Geom::Text2D::GetText() const    { return m_Text; }
uint32_t glib::Geom::Text2D::GetSize() const              { return m_Size; }
const glib::Font* glib::Geom::Text2D::GetFont() const     { return m_Font; }
void glib::Geom::Text2D::SetText(const std::string &text) { m_Text = text; }
void glib::Geom::Text2D::SetSize(uint32_t size)           { m_Size = size; }
void glib::Geom::Text2D::SetFont(const glib::Font *font)  { m_Font = font; }

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE