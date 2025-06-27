#include <utility>

#include "glib.h"

glib::TextureSlotManager::TextureSlotManager() {
    m_MaxSlotsCount = GlCore::ShaderCreator::GetMaxSlotsCount();

    m_Textures.reserve(m_MaxSlotsCount);
    m_Slots.reserve(m_MaxSlotsCount);
}

void glib::TextureSlotManager::BindDrawFunc(std::function<void()> DrawBuffer) {
    m_DrawBuffer = std::move(DrawBuffer);
}

int glib::TextureSlotManager::PushTexture(const GlCore::Texture *texture) {
    for (int i = 0; i < m_Textures.size(); i++) {
        if (m_Textures[i] == texture) {
            return i;
        }
    }

    int slot = static_cast<int>(m_Textures.size());

    if (slot == m_MaxSlotsCount) {
        m_DrawBuffer();
        Clear();
        slot = 0;
    }
    m_Textures.emplace_back(texture);
    return slot;
}

const int* glib::TextureSlotManager::GetSlotsData() {
    for (int i = 0; i < m_Textures.size(); i++) {
        m_Slots.push_back(i);
    }

    return m_Slots.data();
}

int glib::TextureSlotManager::GetMaxSlotsCount() const {
    return m_MaxSlotsCount;
}

void glib::TextureSlotManager::Clear() {
    m_Textures.clear();
    m_Slots.clear();
}

void glib::TextureSlotManager::Bind() {
    for (int i = 0; i < m_Textures.size(); i++) {
        m_Textures[i]->Bind(i);
    }
}