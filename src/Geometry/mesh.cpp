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

Mesh::Mesh(const std::vector<float>& points, const std::vector<uint32_t>& indices)
     : m_Points(points), m_Indices(indices)
{}

Mesh::Mesh(Mesh&& other) noexcept
    : m_Points(std::move(other.m_Points)),
      m_Indices(std::move(other.m_Indices)),
      m_UVCoordinates(std::move(other.m_UVCoordinates)),
      m_Transform(other.m_Transform)
{
    other.m_Transform = {};
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    m_Vertices = std::move(other.m_Vertices);
    m_Indices = std::move(other.m_Indices);
    m_UVCoordinates = std::move(other.m_UVCoordinates);
    m_Transform = other.m_Transform;

    other.m_Transform = {};

    return *this;
}

const std::vector<float>& Mesh::GetPoints() const     { return m_Points;               }
const std::vector<uint32_t>& Mesh::GetIndices() const { return m_Indices;              }
const std::vector<float>& Mesh::GetUV() const         { return m_UVCoordinates;        }
const glm::vec3& Mesh::GetPosition() const            { return m_Transform.position;   }
const glm::vec3& Mesh::GetRotation() const            { return m_Transform.rotation;   }
const glm::vec3& Mesh::GetScale() const               { return m_Transform.scale;      }
const glm::vec3& Mesh::GetDeltaPivot() const          { return m_Transform.deltaPivot; }
const std::vector<Vertex>& Mesh::GetVertices() const  { return m_Vertices;             }
uint32_t Mesh::GetUVSlot() const                      { return m_UVSlot;               }
const std::vector<glm::vec4> &Mesh::GetColor() const  { return m_Colors;               }

void Mesh::SetPoints(const std::vector<float>& points)      { m_Points = points;           }
void Mesh::SetIndices(const std::vector<uint32_t>& indices) { m_Indices = indices;         }
void Mesh::SetUV(const std::vector<float>& uv)              { m_UVCoordinates = uv;        }
void Mesh::SetPosition(const glm::vec3& pos)                { m_Transform.position = pos;  }
void Mesh::SetRotation(const glm::vec3& rot)                { m_Transform.rotation = rot;  }
void Mesh::SetScale(const glm::vec3& scale)                 { m_Transform.scale = scale;   }
void Mesh::SetUVSlot(uint32_t slot)                         { m_UVSlot = slot;             }
void Mesh::SetColor(const std::vector<glm::vec4>& colors)   { m_Colors = colors;           }
void Mesh::SetDeltaPivot(const glm::vec3 &dp)               { m_Transform.deltaPivot = dp; }

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

static std::vector<Vertex> vertices;
std::vector<Vertex> MeshBaker::Bake(const Geom::Mesh &mesh) {
    auto& trans = mesh.m_Transform;
    auto& points = mesh.m_Points;
    auto& colors = mesh.m_Colors;

    vertices.clear();
    vertices.resize(points.size() / 3);

    glm::mat4 tm(1.0f);
    Basis basis;

    tm = glm::translate(tm, trans.deltaPivot + trans.position);

    tm = glm::rotate(tm, glm::radians(trans.rotation.x), basis.xAxis);
    basis.yAxis = rotate_about_vec(basis.yAxis, basis.xAxis, -glm::radians(trans.rotation.x));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.xAxis, -glm::radians(trans.rotation.x));

    tm = glm::rotate(tm, glm::radians(trans.rotation.y), basis.yAxis);
    basis.xAxis = rotate_about_vec(basis.xAxis, basis.yAxis, -glm::radians(trans.rotation.y));
    basis.zAxis = rotate_about_vec(basis.zAxis, basis.yAxis, -glm::radians(trans.rotation.y));

    tm = glm::rotate(tm, glm::radians(trans.rotation.z), basis.zAxis);
    basis.xAxis = rotate_about_vec(basis.xAxis, basis.zAxis, -glm::radians(trans.rotation.z));
    basis.yAxis = rotate_about_vec(basis.yAxis, basis.zAxis, -glm::radians(trans.rotation.z));

    tm = glm::translate(tm, -trans.deltaPivot - trans.position);
    tm = glm::translate(tm, trans.position);

    tm = glm::scale(tm, trans.scale);

    for (uint32_t i = 0; i < points.size(); i+=3) {
        glm::vec4 p(points[i], points[i + 1], points[i + 2], 1.0f);
        p = tm * p;

        int k = i / 3;
        vertices[k].position = {p[0], p[1], p[2]};
        vertices[k].color = (k < colors.size()) ? colors[k] : glm::vec4(1.0f);
    }

    return vertices;
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
        m.SetDeltaPivot({0.5f, 0.5f, 0.0f});

        return m;
    }));
}

MeshFactory::MeshFactory() {
    init_quad();
}



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE