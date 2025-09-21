#pragma once

GLIB_NAMESPACE_OPEN
ECS_NAMESPACE_OPEN

template<class T, class ...Args>
void Entity::AddComponent(Args... args) {
    for (auto &it : m_Components) {
        if (T* element = dynamic_cast<T*>(it.get())) {
            Logger::LogErr("Component", "This component already exists!");
            return;
        }
    }

    m_Components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
}


template<class T>
const T& Entity::GetComponent() const {
    for (auto &it : m_Components) {
        if (T* element = dynamic_cast<T*>(it.get())) {
            return *element;
        }
    }

    Logger::LogWar("Component", "This component doesn't exist! Program will return basic component");
    return *(dynamic_cast<T*>(m_BasicComponent.get()));
}


template<class T>
T &Entity::SetComponent() {
    for (auto &it : m_Components) {
        if (T* element = dynamic_cast<T*>(it.get())) {
            // TODO: changing of the mesh for each component
            return *element;
        }
    }

    Logger::LogWar("Component", "This component doesn't exist! Program will return basic component");
    return *(dynamic_cast<T*>(m_BasicComponent.get()));
}


ECS_NAMESPACE_CLOSE
GLIB_NAMESPACE_CLOSE