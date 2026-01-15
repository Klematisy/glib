#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Entity {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Transform> transition;
    std::shared_ptr<Material> material;
};

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE