#include "entity.h"

GLIB_NAMESPACE_USING;
using namespace Geom;

Entity::Entity(std::shared_ptr<Mesh> mesh,
               std::shared_ptr<Transform> t,
               std::shared_ptr<Material> mat)
   : mesh(std::move(mesh)), transform(std::move(t)), material(std::move(mat))
{}