#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"

VLADLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Entity {
    Entity() = default;

    Entity(std::unique_ptr<Mesh>, std::unique_ptr<Transform>, std::unique_ptr<Material>);
    Entity(const Entity& e) {
        if (e.mesh) mesh = std::make_unique<Mesh>(*e.mesh);
        if (e.transform) transform = std::make_unique<Transform>(*e.transform);
        if (e.material) material = std::make_unique<Material>(*e.material);
    }

    Entity& operator=(const Entity& e) {
        if (e.mesh) mesh = std::make_unique<Mesh>(*e.mesh);
        if (e.transform) transform = std::make_unique<Transform>(*e.transform);
        if (e.material) material = std::make_unique<Material>(*e.material);
        return *this;
    }

    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Transform> transform;
    std::unique_ptr<Material> material;
};

GEOM_NAMESPACE_CLOSE
VLADLIB_NAMESPACE_CLOSE