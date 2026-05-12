#include "entity.h"

VLADLIB_NAMESPACE_USING;
using namespace Geom;

Entity::Entity(std::unique_ptr<Mesh> mesh,
               std::unique_ptr<Transform> t,
               std::unique_ptr<Material> mat)
   : mesh(std::move(mesh)), transform(std::move(t)), material(std::move(mat))
{}

Entity::Entity(const Entity& other) {
    if (other.mesh) mesh = std::make_unique<Mesh>(*other.mesh);
    if (other.transform) transform = std::make_unique<Transform>(*other.transform);
    if (other.material) material = std::make_unique<Material>(*other.material);
}

Entity::Entity(Entity&& other) noexcept
    : mesh(std::move(other.mesh)),
      transform(std::move(other.transform)),
      material(std::move(other.material))
{}

Entity& Entity::operator=(const Entity& other) {
    if (other.mesh) mesh = std::make_unique<Mesh>(*other.mesh);
    if (other.transform) transform = std::make_unique<Transform>(*other.transform);
    if (other.material) material = std::make_unique<Material>(*other.material);

    return *this;
}

Entity& Entity::operator=(Entity&& other) noexcept {
    mesh = std::move(other.mesh);
    transform = std::move(other.transform);
    material = std::move(other.material);

    return *this;
}