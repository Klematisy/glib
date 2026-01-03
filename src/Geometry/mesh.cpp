#include "mesh.h"

#include <utility>

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

static glm::vec3 rotate_about_vec(const glm::vec3& src,
                                  const glm::vec3& axis,
                                  float angle) {
    float cos = std::cosf(angle);
    float sin = std::sinf(angle);
    return src * cos + glm::cross(axis, src) * sin + axis * glm::dot(axis, src) * (1 - cos);
}

Mesh::Mesh(const std::vector<glm::vec3>& points, const std::vector<uint32_t>& indices)
     : points(points), indices(indices)
{}

Mesh::Mesh(Mesh&& other) noexcept
    : points(std::move(other.points)),
      indices(std::move(other.indices))
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    points = std::move(other.points);
    indices = std::move(other.indices);

    return *this;
}

MeshFactory& MeshFactory::Get() {
    static MeshFactory meshFactory;
    return meshFactory;
}

void MeshFactory::AddMesh(const std::string& name, std::function<Mesh()> functor) {
    using namespace std::string_literals;

    if (m_Meshes.find(name) != m_Meshes.cend()) {
        Logger::LogErr("MeshFactory", "The name '"s + name + "' already exists!");
        return;
    }

    m_Meshes[name] = std::move(functor);
}

Mesh MeshFactory::CreateMesh(const std::string& name) {
    using namespace std::string_literals;
    if (m_Meshes.find(name) == m_Meshes.cend()) {
        Logger::LogErr("MeshFactory", "The name '"s + name + "' doesn't exists! CreateMesh will return basic mesh");
        return Mesh();
    }

    return std::move(m_Meshes[name]());
}


void MeshFactory::init_quad() {
    AddMesh("quad", std::function<Mesh()> ([](){
        Mesh m({
           {0.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f},
           {1.0f, 1.0f, 0.0f},
           {1.0f, 0.0f, 0.0f}
        }, {
             0, 1, 2,
             2, 3, 0
        });
        return m;
    }));
}

void MeshFactory::init_cube() {
    AddMesh("cube", std::function<Mesh()> ([](){
        Mesh m({
           {-0.5f,  0.5f,  0.5f},
           {-0.5f,  0.5f, -0.5f},
           {-0.5f, -0.5f, -0.5f},
           {-0.5f, -0.5f,  0.5f},

           { 0.5f,  0.5f,  0.5f},
           { 0.5f,  0.5f, -0.5f},
           { 0.5f, -0.5f, -0.5f},
           { 0.5f, -0.5f,  0.5f},

           { 0.5f,  0.5f, -0.5f},
           {-0.5f,  0.5f, -0.5f},
           {-0.5f, -0.5f, -0.5f},
           { 0.5f, -0.5f, -0.5f},

           { 0.5f,  0.5f,  0.5f},
           {-0.5f,  0.5f,  0.5f},
           {-0.5f, -0.5f,  0.5f},
           { 0.5f, -0.5f,  0.5f},

           {-0.5f, -0.5f, -0.5f},
           {-0.5f, -0.5f,  0.5f},
           { 0.5f, -0.5f,  0.5f},
           { 0.5f, -0.5f, -0.5f},

           {-0.5f,  0.5f, -0.5f},
           {-0.5f,  0.5f,  0.5f},
           { 0.5f,  0.5f,  0.5f},
           { 0.5f,  0.5f, -0.5f},
        }, {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,

             8,  9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,

            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        });
        return m;
    }));
}

MeshFactory::MeshFactory() {
    init_quad();
    init_cube();
}



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE