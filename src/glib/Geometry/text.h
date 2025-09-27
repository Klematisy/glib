#pragma once

#include <string>

#include "environment.h"
#include "mesh.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN


class Text2D {
public:
    Text2D(const std::string& str = "");

    void SetText(const std::string& text);
    const std::string& GetText() const;
private:
    std::string m_Text;
    std::vector<std::unique_ptr<Mesh>> quads;
};



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE