#pragma once

#include <memory>

#include "environment.h"
#include "entity.h"
#include "Geometry/mesh.h"

#include "glm/glm.hpp"

GLIB_NAMESPACE_OPEN
ECS_NAMESPACE_OPEN

class Component {
public:
    virtual ~Component() = default;
};

class Position : public Component {
public:
    Position() = default;
    Position(float x = 0, float y = 0, float z = 0) : m_Position(x, y, z) {}

    const glm::vec3& GetPos() const {return m_Position;}
    void SetPos(const glm::vec3& pos) {m_Position = pos;}

    ~Position() override = default;
private:
    glm::vec3 m_Position {0.0f, 0.0f, 0.0f};
};

class Rotate {
public:
    float GetPos() const {return m_Rotate;}
    void SetPos(float r) {m_Rotate = r;}
private:
    float m_Rotate = 0.0f;
};

class XRotate : public Component, public Rotate {~XRotate() override = default;};
class YRotate : public Component, public Rotate {~YRotate() override = default;};
class ZRotate : public Component, public Rotate {~ZRotate() override = default;};

class MeshComponent : public Component {
public:
    MeshComponent() = default;
    MeshComponent(const Mesh::Mesh& m) {*mesh = m;}

    std::unique_ptr<Mesh::Mesh> mesh = std::make_unique<Mesh::Mesh>();
    ~MeshComponent() override = default;
};

class MaterialComponent : public Component {
public:
    // TODO: Create Material class
    ~MaterialComponent() override = default;
};

ECS_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE