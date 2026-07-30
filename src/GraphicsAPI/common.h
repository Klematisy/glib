#pragma once

#include <cinttypes>
#include <cstdlib>

#define PUBLIC public:
#define READONLY public:
#define PRIVATE
#define PROTECTED
// #define PRIVATE private:
// #define PROTECTED protected:


using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u0  = void;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

struct Rectanglei {
    i32 x, y, width, height;
};

struct Rectanglef {
    float x, y, width, height;
};

struct Rectangled {
    double x, y, width, height;
};

class Range {
public:
    inline Range() = default;

    inline Range(u32 size) {
        data = (u8*)std::calloc(size, 1);
        r_Capacity = size;
    }

    inline Range(const Range& other) = delete;
    inline Range& operator=(const Range& other) = delete;

    inline Range(Range&& other) noexcept {
        if (data) {
            std::free(data);
        }

        data = other.data;
        r_Capacity = other.r_Capacity;
        other.data = nullptr;
        other.r_Capacity = 0;
    }

    inline Range& operator=(Range&& other) noexcept {
        if (data) {
            std::free(data);
        }

        data = other.data;
        r_Capacity = other.r_Capacity;
        other.data = nullptr;
        other.r_Capacity = 0;

        return *this;
    }

    inline ~Range() {
        if (data) std::free(data);
        data = nullptr;
    }

    u8* data = nullptr;
READONLY
    u32 r_Capacity = 0;
};

template<class T>
class HeapArray {
public:
    inline HeapArray() = default;

    inline u0 Resize(u32 size) {
        m_Range = Range(sizeof(T) * size);
        m_Size = size;
    }

    inline u0 Reverse(u32 size) {
        m_Range = Range(sizeof(T) * size);
    }

    inline u32 Capacity() const {
        return m_Range.r_Capacity;
    }

    inline u0 PushBack(T element) {
        m_Range.data[sizeof(T) * m_Size++] = element;
    }

    inline u0 Clear() {
        m_Size = 0;
    }

    inline T& operator[](u32 i) {
        if (m_Size < i) {
            return *(T*)m_Range.data;
        }
        
        return *(T*)(m_Range.data + sizeof(T) * i);
    }

    inline const T& operator[](u32 i) const {
        if (m_Size < i) {
            return *(T*)m_Range.data;
        }
        return *(T*)m_Range.data + sizeof(T) * i;
    }
private:
    Range m_Range;
    u32 m_Size = 0;
};
