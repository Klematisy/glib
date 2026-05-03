#include "entity.h"

VLADLIB_NAMESPACE_USING;
using namespace Geom;

Entity::Entity(std::unique_ptr<Mesh> mesh,
               std::unique_ptr<Transform> t,
               std::unique_ptr<Material> mat)
   : mesh(std::move(mesh)), transform(std::move(t)), material(std::move(mat))
{}