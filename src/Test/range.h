#pragma once
#include <cstdlib>

class MemoryRange {
public:
    MemoryRange() = default;

    MemoryRange(size_t size) {
        m_Data = (char*)std::calloc(size, 1);
        m_Capacity = size;
    }

    MemoryRange(const MemoryRange& other) = delete;
    MemoryRange& operator=(const MemoryRange& other) = delete;

    MemoryRange(MemoryRange&& other) noexcept {
        if (m_Data) {
            std::free(m_Data);
        }

        m_Data = other.m_Data;
        m_Capacity = other.m_Capacity;
        other.m_Data = nullptr;
        m_Capacity = 0;
    }

    MemoryRange& operator=(MemoryRange&& other) noexcept {
        if (m_Data) {
            std::free(m_Data);
        }

        m_Data = other.m_Data;
        m_Capacity = other.m_Capacity;
        other.m_Data = nullptr;
        m_Capacity = 0;

        return *this;
    }

    size_t Capacity() const {
        return m_Capacity;
    }

    char* operator[](uint32_t i) {
        return m_Data + i;
    }

    const char* operator[](uint32_t i) const {
        return m_Data + i;
    }

    ~MemoryRange() {
        if (m_Data) std::free(m_Data);
    }
private:
    char* m_Data = nullptr;
    size_t m_Capacity = 0;
};

template<class T>
class HeapArray {
public:
    HeapArray() = default;

    void Resize(size_t size) {
        m_Range = MemoryRange(sizeof(T) * size);
        m_Size = size;
    }

    void Reverse(size_t size) {
        m_Range = MemoryRange(sizeof(T) * size);
    }

    size_t Capacity() const {
        return m_Range.Capacity();
    }

    void PushBack(T element) {
        m_Range[sizeof(T) * m_Size++] = element;
    }

    void Clear() {
        m_Size = 0;
    }

    T& operator[](uint32_t i) {
        if (m_Size < i) {
            return *(T*)m_Range[0];
        }
        return *(T*)(m_Range[sizeof(T) * i]);
    }

    const T& operator[](uint32_t i) const {
        if (m_Size < i) {
            return *(T*)m_Range[0];
        }
        return *(T*)m_Range[sizeof(T) * i];
    }
private:
    MemoryRange m_Range;
    uint32_t m_Size = 0;
};
