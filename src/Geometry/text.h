#pragma once

#include <string>

#include "environment.h"
#include "mesh.h"
#include "FontGenerator/font_generator.h"

VLADLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN


class Text2D {
public:
    explicit Text2D(const std::string& str,
                    const Font* font,
                    uint32_t size);

    const std::string& GetText() const;
    uint32_t GetSize() const;
    const Font* GetFont() const;

    std::shared_ptr<Mesh> ReadMesh() const;
    std::shared_ptr<Mesh> WriteMesh();

    const glm::vec3& GetTextScreenSize() const;

    void SetText(const std::string& text);
    void SetSize(uint32_t size);
    void SetFont(const Font* font);
private:
    mutable bool m_Dirty = true;
    mutable glm::vec3 m_TextScreenSize = glm::vec3(0.0f);

    std::string m_Text;
    const Font* m_Font = nullptr;
    uint32_t m_Size = 0;
    std::shared_ptr<Mesh> m_Mesh;
};



GEOM_NAMESPACE_CLOSE
VLADLIB_NAMESPACE_CLOSE