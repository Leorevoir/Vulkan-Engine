#pragma once

#include <Engine/Types.hpp>
#include <Maths/Maths.hpp>

#include <algorithm>

namespace vk::maths {

template<u32 N, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_ASSERT_NUMBERS(N, T))

struct Vector {
    public:
        VK_MATHS_BASE_TEMPLATE(N, T)

        constexpr explicit inline Vector(const T &value)
        {
            std::fill(_data.begin(), _data.end(), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N && (std::convertible_to<Args, T> && ...))
        constexpr explicit inline Vector(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires std::convertible_to<U, T>
        constexpr explicit Vector(const Vector<N, U> &other)
        {
            for (size_type i = 0; i < N; ++i) {
                _data[i] = static_cast<T>(other[i]);
            }
        }

        constexpr Vector operator+() const
        {
            return *this;
        }

    private:
        T _data[N] = {0};
};

/**
 * binary operators
 */

template<u32 N, typename T>
constexpr Vector<N, T> operator+(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> result = lhs;

    result += rhs;
    return result;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator-(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> result = lhs;

    result -= rhs;
    return result;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator*(const Vector<N, T> &vec, const T &scalar)
{
    Vector<N, T> result = vec;

    result *= scalar;
    return result;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator*(const T &scalar, const Vector<N, T> &vec)
{
    return vec * scalar;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator/(const Vector<N, T> &vec, const T &scalar)
{
    Vector<N, T> result = vec;

    result /= scalar;
    return result;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator*(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> result = lhs;

    result *= rhs;
    return result;
}

template<u32 N, typename T>
constexpr Vector<N, T> operator/(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    Vector<N, T> result = lhs;

    result /= rhs;
    return result;
}

template<u32 N, typename T>
constexpr T dot(const Vector<N, T> &lhs, const Vector<N, T> &rhs)
{
    return lhs.dot(rhs);
}

template<typename T>
constexpr Vector<3, T> cross(const Vector<3, T> &lhs, const Vector<3, T> &rhs)
{
    return lhs.cross(rhs);
}

template<u32 N, typename T>
T length(const Vector<N, T> &vec)
{
    return vec.length();
}

template<u32 N, typename T>
constexpr T length_squared(const Vector<N, T> &vec)
{
    return vec.length_squared();
}

template<u32 N, typename T>
Vector<N, T> normalize(const Vector<N, T> &vec)
{
    return vec.normalized();
}

/**
 * instanciations
 */

using Vec2f = Vector<2, f32>;
using Vec3f = Vector<3, f32>;
using Vec4f = Vector<4, f32>;

using Vec2d = Vector<2, f64>;
using Vec3d = Vector<3, f64>;
using Vec4d = Vector<4, f64>;

using Vec2i = Vector<2, i32>;
using Vec3i = Vector<3, i32>;
using Vec4i = Vector<4, i32>;

}// namespace vk::maths
