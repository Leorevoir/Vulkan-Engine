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

        VKM_API constexpr explicit Vector(const T &value)
        {
            std::fill(_data.begin(), _data.end(), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N && (VK_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VKM_API constexpr explicit Vector(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires(VK_MATHS_ASSERT_CONVERTIBLE(U, T))
        VKM_API constexpr explicit Vector(const Vector<N, U> &other)
        {
            for (size_type i = 0; i < N; ++i) {
                _data[i] = static_cast<T>(other[i]);
            }
        }

        VKM_API VKE_NODISCARD constexpr Vector operator+() const
        {
            return *this;
        }

        VKM_API VKE_NODISCARD constexpr T &operator[](size_type index)
        {
            return _data[index];
        }
        VKM_API VKE_NODISCARD constexpr const T &operator[](size_type index) const
        {
            return _data[index];
        }

    protected:
        T _data[N] = {0};
};

/**
 * concrete vector types
 */

template<typename T>
struct Vector2 final : public Vector<2, T> {
        using Vector<2, T>::Vector;
        using Vector<2, T>::_data;

        T &x = _data[0];
        T &y = _data[1];
        T &r = _data[0];
        T &g = _data[1];
};

template<typename T>
struct Vector3 final : public Vector<3, T> {
        using Vector<3, T>::Vector;
        using Vector<3, T>::_data;

        T &x = _data[0];
        T &y = _data[1];
        T &z = _data[2];
        T &r = _data[0];
        T &g = _data[1];
        T &b = _data[2];
};

template<typename T>
struct Vector4 final : public Vector<4, T> {
        using Vector<4, T>::Vector;
        using Vector<4, T>::_data;

        T &x = _data[0];
        T &y = _data[1];
        T &z = _data[2];
        T &w = _data[3];
        T &r = _data[0];
        T &g = _data[1];
        T &b = _data[2];
        T &a = _data[3];
};

/**
 * instanciations
 */

using Vec2f = Vector2<f32>;
using Vec3f = Vector3<f32>;
using Vec4f = Vector4<f32>;

using Vec2d = Vector2<f64>;
using Vec3d = Vector3<f64>;
using Vec4d = Vector4<f64>;

using Vec2i = Vector2<i32>;
using Vec3i = Vector3<i32>;
using Vec4i = Vector4<i32>;

}// namespace vk::maths
