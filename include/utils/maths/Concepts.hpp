#pragma once

#include <utils/Types.hpp>

#include <concepts>
#include <type_traits>

namespace lumen {

namespace concepts {

template<usize N, typename T>
concept Vec = (N >= 2 && N <= 4) && std::is_arithmetic_v<T>;

template<typename... Args>
concept AllArithmetic = (std::is_arithmetic_v<Args> && ...);

template<typename T, typename... Args>
concept AllConvertibleTo = (std::convertible_to<Args, T> && ...);

template<typename T, typename... Args>
concept IsSameAs = (std::same_as<T, Args> && ...);

template<typename T>
concept IsFloatingPoint = std::is_floating_point_v<T>;

}// namespace concepts

}// namespace lumen
