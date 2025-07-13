#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace vk {

struct NonCopyable {

        inline constexpr NonCopyable() = default;

        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;

        NonCopyable(NonCopyable &&) = default;
        NonCopyable &operator=(NonCopyable &&) = default;
};

struct NonMovable {

        inline constexpr NonMovable() = default;

        NonMovable(const NonMovable &&) = delete;
        NonMovable &operator=(const NonMovable &&) = delete;
};

}// namespace vk

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using isize = std::make_signed_t<size_t>;
using usize = std::make_unsigned_t<size_t>;
