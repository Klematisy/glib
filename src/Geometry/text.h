#pragma once

#include <string>

#include "../environment.h"
#include "mesh.h"
#include "../FontGenerator/font_generator.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN


class Text2D {
public:
    explicit Text2D(const std::string& str,
                    const Font* font,
                    uint32_t size);

    const std::string& GetText() const;
    uint32_t GetSize() const;
    const Font* GetFont() const;

    void SetText(const std::string& text);
    void SetSize(uint32_t size);
    void SetFont(const Font* font);
private:
    std::string m_Text;
    const Font* m_Font = nullptr;
    uint32_t m_Size = 0;
    std::vector<std::unique_ptr<Mesh>> m_Quads;
};



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE