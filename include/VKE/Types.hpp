#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace vke {

struct NonCopyable {

        inline constexpr NonCopyable() = default;

        inline constexpr NonCopyable(const NonCopyable &) = delete;
        inline constexpr NonCopyable &operator=(const NonCopyable &) = delete;

        inline constexpr NonCopyable(NonCopyable &&) = default;
        inline constexpr NonCopyable &operator=(NonCopyable &&) = default;
};

struct NonMovable {

        inline constexpr NonMovable() = default;

        inline constexpr NonMovable(const NonMovable &&) = delete;
        inline constexpr NonMovable &operator=(const NonMovable &&) = delete;
};

}// namespace vke

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
