#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"

VLADLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

class Entity {
public:
    Entity() = default;

    Entity(std::unique_ptr<Mesh>, std::unique_ptr<Transform>, std::unique_ptr<Material>);
    Entity(const Entity& other);
    Entity(Entity&& other) noexcept;

    Entity& operator=(const Entity& other);
    Entity& operator=(Entity&& other) noexcept;

    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Transform> transform;
    std::unique_ptr<Material> material;
};

GEOM_NAMESPACE_CLOSE
VLADLIB_NAMESPACE_CLOSE