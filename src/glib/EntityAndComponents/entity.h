#pragma once

#include <memory>

#include "environment.h"

#define ECS_NAMESPACE_OPEN namespace ECS {
#define ECS_NAMESPACE_CLOSE }
#define ECS_NAMESPACE_USING using namespace ECS

#include "components.h"

template<class T>
using vec = std::vector<T>;
template<class T>
using uptr = std::unique_ptr<T>;

GLIB_NAMESPACE_OPEN
ECS_NAMESPACE_OPEN

class Entity {
public:
    template<class T, class... Args>
    void AddComponent(Args... args);

    template<class T>
    const T& GetComponent() const;

    template<class T>
    T& SetComponent();
private:
    uptr<Component> m_BasicComponent = std::make_unique<Component>();
    vec<uptr<Component>> m_Components;
};

ECS_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE

#include "entity.inl"