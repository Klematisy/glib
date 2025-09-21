#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "Draw/structs.h"
#include "environment.h"
#include "Logger/logger.h"

#define MESH_NAMESPACE_OPEN  namespace Mesh {
#define MESH_NAMESPACE_CLOSE }

GLIB_NAMESPACE_OPEN
MESH_NAMESPACE_OPEN

class Mesh {
public:
    Mesh() = default;
    Mesh(const Mesh& other) = default;
    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(const Mesh& other) = default;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(const std::vector<float>& vertices,
         const std::vector<uint32_t>& indices);

    const std::vector<float>& GetVertices() const;
    const std::vector<uint32_t>& GetIndices() const;
    const std::vector<float>& GetUV() const;

    void SetVertices(const std::vector<float>& vertices);
    void SetIndices(const std::vector<uint32_t>& indices);
    void SetUV(const std::vector<float>& uv);
protected:
    std::vector<float> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<float> m_UVCoordinates;
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



MESH_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE