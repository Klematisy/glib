#include "mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
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




Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
     : m_Vertices(vertices), m_Indices(indices)
{}

Mesh::Mesh(Mesh&& other) noexcept
    : m_Vertices(std::move(other.m_Vertices)),
      m_Indices(std::move(other.m_Indices)),
      m_UVCoordinates(std::move(other.m_UVCoordinates))
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    m_Vertices = std::move(other.m_Vertices);
    m_Indices = std::move(other.m_Indices);
    m_UVCoordinates = std::move(other.m_UVCoordinates);

    return *this;
}

const std::vector<float>& Mesh::GetVertices() const { return m_Vertices; }
const std::vector<uint32_t>& Mesh::GetIndices() const { return m_Indices; }
const std::vector<float>& Mesh::GetUV() const { return m_UVCoordinates; }

void Mesh::SetVertices(const std::vector<float>& vertices) { m_Vertices = vertices; }
void Mesh::SetIndices(const std::vector<uint32_t>& indices) { m_Indices = indices; }
void Mesh::SetUV(const std::vector<float>& uv) { m_UVCoordinates = uv; }

const glm::vec3& Mesh::GetPosition() const { return m_Transform.position; }
const glm::vec3& Mesh::GetRotation() const { return m_Transform.rotation; }
const glm::vec3& Mesh::GetScale() const { return m_Transform.scale; }

void Mesh::SetPosition(const glm::vec3& pos) {
    m_Transform.position = pos;
    m_Dirty = true;
}
void Mesh::SetRotation(const glm::vec3& rot) {
    m_Transform.rotation = rot;
    m_Dirty = true;
}
void Mesh::SetScale(const glm::vec3& scale) {
    m_Transform.scale = scale;
    m_Dirty = true;
}


std::vector<float> Mesh::Bake() const {
    std::vector<float> newVertices(m_Vertices);

    auto &trans = m_Transform;

    glm::mat4 tm(1.0f);
    Basis basis;

    tm = glm::scale(tm, trans.scale);

    tm = glm::rotate(tm, glm::radians(trans.rotation.x), basis.xAxis);
    basis.yAxis = rotate_about_vec(basis.yAxis, basis.xAxis, glm::radians(trans.rotation.x));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.xAxis, glm::radians(trans.rotation.x));

    tm = glm::rotate(tm, glm::radians(trans.rotation.y), basis.yAxis);
    basis.xAxis = rotate_about_vec(basis.xAxis, basis.yAxis, glm::radians(trans.rotation.x));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.yAxis, glm::radians(trans.rotation.x));

    tm = glm::rotate(tm, glm::radians(trans.rotation.z), basis.zAxis);
    basis.xAxis = rotate_about_vec(basis.xAxis, basis.zAxis, glm::radians(trans.rotation.x));
    basis.yAxis = rotate_about_vec(basis.yAxis, basis.zAxis, glm::radians(trans.rotation.x));

    tm = glm::translate(tm, trans.position);

//    for (uint32_t i = 0; i < 16; i++) {
//        if (i % 4 == 0) {
//            std::cout << std::endl;
//        }
//        std::cout << tm[i % 4][(int)i / 4] << " ";
//    }


    for (uint32_t i = 0; i < newVertices.size(); i+=3) {
        glm::vec4 v(newVertices[i], newVertices[i + 1], newVertices[i + 2], 1.0f);
        v = tm * v;

        newVertices[i] = v[0];
        newVertices[i + 1] = v[1];
        newVertices[i + 2] = v[2];
    }


    m_Dirty = false;
    return std::move(newVertices);
}





MeshFactory &MeshFactory::Get() {
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
             0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f,
             1.0f, 1.0f, 0.0f,
             1.0f, 0.0f, 0.0f
        }, {
             0, 1, 2,
             2, 3, 0
        });

        m.SetUV({0.0f, 0.0f,
                 0.0f, 1.0f,
                 1.0f, 1.0f,
                 1.0f, 0.0f});
        return m;

    }));
}

MeshFactory::MeshFactory() {
    init_quad();
};



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE