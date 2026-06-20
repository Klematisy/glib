#pragma once

#include <cinttypes>

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
