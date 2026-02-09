#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Entity {
    Entity() = default;

    Entity(std::shared_ptr<Mesh>, std::shared_ptr<Transform>, std::shared_ptr<Material>);
    Entity(const Entity& e) {
        if (e.mesh) mesh = std::make_shared<Mesh>(*e.mesh);
        if (e.transform) transform = std::make_shared<Transform>(*e.transform);
        if (e.material) material = std::make_shared<Material>(*e.material);
    }

    Entity& operator=(const Entity& e) {
        if (e.mesh) mesh = std::make_shared<Mesh>(*e.mesh);
        if (e.transform) transform = std::make_shared<Transform>(*e.transform);
        if (e.material) material = std::make_shared<Material>(*e.material);
        return *this;
    }

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Material> material;
};

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE