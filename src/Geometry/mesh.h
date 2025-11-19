#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Draw/structs.h"
#include "environment.h"
#include "Logger/logger.h"

#define GEOM_NAMESPACE_OPEN  namespace Geom {
#define GEOM_NAMESPACE_CLOSE }

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Basis {
    glm::vec3 xAxis {1, 0, 0};
    glm::vec3 yAxis {0, 1, 0};
    glm::vec3 zAxis {0, 0, 1};
};

struct Transform {
    glm::vec3 position {0};
    glm::vec3 rotation {0};
    glm::vec3 scale {1};
    glm::vec3 deltaPivot {0};
    glm::mat4 model {1.0f};
};

class Mesh {
public:
    Mesh() = default;
    Mesh(const Mesh&) = default;
    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(const std::vector<float>& vertices,
         const std::vector<uint32_t>& indices);

    const std::vector<float>& GetPoints() const;
    const std::vector<uint32_t>& GetIndices() const;
    const std::vector<float>& GetUV() const;

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetDeltaPivot() const;
    const std::vector<Vertex>& GetVertices() const;

    void SetPoints(const std::vector<float>& vertices);
    void SetIndices(const std::vector<uint32_t>& indices);
    void SetUV(const std::vector<float>& uv);

    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);
    void SetScale(const glm::vec3& scale);
    void SetDeltaPivot(const glm::vec3& dp);

    std::vector<Vertex>& Bake() const;
private:
    mutable std::vector<Vertex> m_Vertices;

    mutable bool m_Dirty = true;

    std::vector<float> m_Points;
    std::vector<uint32_t> m_Indices;
    std::vector<float> m_UVCoordinates;

    Transform m_Transform {};
};


class MeshFactory {
public:
    static MeshFactory& Get();

    void AddMesh(const std::string& name, std::function<Mesh()> functor);
    Mesh CreateMesh(const std::string& name);
private:
    void init_quad();

    MeshFactory();
    std::unordered_map<std::string, std::function<Mesh()>> m_Meshes;
};



GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE