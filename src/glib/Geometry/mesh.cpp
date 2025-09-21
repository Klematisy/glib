#include "mesh.h"

GLIB_NAMESPACE_OPEN
MESH_NAMESPACE_OPEN


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

const std::vector<float>& Mesh::GetVertices() const {
    return m_Vertices;
}

const std::vector<uint32_t>& Mesh::GetIndices() const {
    return m_Indices;
}

const std::vector<float>& Mesh::GetUV() const {
    return m_UVCoordinates;
}

void Mesh::SetVertices(const std::vector<float>& vertices) {
    m_Vertices = vertices;
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices) {
    m_Indices = indices;
}

void Mesh::SetUV(const std::vector<float>& uv) {
    m_UVCoordinates = uv;
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

    m_Meshes[name] = functor;
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
        return
        Mesh({
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        }, {
            0, 1, 2,
            2, 3, 0
        });
    }));
}

MeshFactory::MeshFactory() {
    init_quad();
};



MESH_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE