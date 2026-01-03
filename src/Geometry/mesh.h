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

struct Mesh {
    friend class MeshBaker;

    Mesh() = default;
    Mesh(const Mesh&) = default;
    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<uint32_t>& indices);

public:
    std::vector<glm::vec3> points;
    std::vector<uint32_t> indices;
};


class MeshFactory {
public:
    static MeshFactory& Get();

    void AddMesh(const std::string& name, std::function<Mesh()> functor);
    Mesh CreateMesh(const std::string& name);
private:
    void init_quad();
    void init_cube();

    MeshFactory();
    std::unordered_map<std::string, std::function<Mesh()>> m_Meshes;
};

GEOM_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE