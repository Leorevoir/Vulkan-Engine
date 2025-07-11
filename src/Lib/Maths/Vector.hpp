#pragma once

#include <Lib/Maths/Base.hpp>
#include <VK/Types.hpp>

#include <algorithm>
#include <cmath>

namespace vk::maths {

template<usize N, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_ASSERT_NUMBERS(T))

struct Vector {
    public:
        VK_MATHS_VEC_TEMPLATE(N, T)

        VKM_API VK_NODISCARD VK_INLINE constexpr Vector() = default;

        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Vector(const T &value)
        {
            std::fill(_data.begin(), _data.end(), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N && (VK_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Vector(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires(VK_MATHS_ASSERT_CONVERTIBLE(U, T))
        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Vector(const Vector<N, U> &other)
        {
            for (size_type i = 0; i < N; ++i) {
                _data[i] = static_cast<T>(other[i]);
            }
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr Vector operator+() const
        {
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector operator-() const
        {
            Vector t;
            for (size_type i = 0; i != N; ++i) {
                t[i] = -_data[i];
            }
            return t;
        }

        VKM_API VK_INLINE constexpr Vector &operator*=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] *= t;
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator/=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] /= t;
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator+=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] += t;
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator-=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] -= t;
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator*=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] *= v[i];
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator/=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] /= v[i];
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator+=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] += v[i];
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator-=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] -= v[i];
            }
            return *this;
        }

        VKM_API VK_INLINE constexpr Vector &operator=(const T &v)
        {
            for (size_type i = 0; i != N; ++i) {
                _data[i] = v;
            }
            return *this;
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T &operator[](size_type index)
        {
            return _data[index];
        }
        VKM_API VK_NODISCARD VK_INLINE constexpr const T &operator[](size_type index) const
        {
            return _data[index];
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T dot(const Vector &o) const
        {
            T sum = 0;

            for (size_type i = 0; i != N; ++i) {
                sum += _data[i] * o[i];
            }
            return sum;
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr Vector cross(const Vector &o) const
        {
            Vector v;

            for (size_type i = 0; i != N; ++i) {
                v[i] = _data[(i + 1) % N] * o[(i + 2) % N] - _data[(i + 2) % N] * o[(i + 1) % N];
            }
            return v;
        }

        VKM_API VK_INLINE constexpr bool is_zero() const
        {
            for (const auto &value : _data) {
                if (value != 0) {
                    return false;
                }
            }
            return true;
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T length() const
        {
            return std::sqrt(dot(*this));
        }

        VKM_API VK_INLINE constexpr void normalize()
        {
            if (!is_zero()) {
                operator*=(1.f / length());
            }
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr Vector normalized() const
        {
            Vector v(*this);

            v.normalize();
            return v;
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T *data() const
        {
            return _data;
        }

    private:
        T _data[N] = {0};
};

template<usize N, typename T>
VKM_API VK_INLINE constexpr auto operator+(const Vector<N, T> &v, const T &t)
{
    Vector<N, T> vec(v);

    vec += t;
    return vec;
}

template<usize N, typename T, typename L>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator+(const L &l, const Vector<N, T> &v)
{
    return v + l;
}

template<usize N, typename T>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator+(Vector<N, T> a, const Vector<N, T> &b)
{
    a += b;
    return a;
}

template<usize N, typename T, typename R>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator*(const Vector<N, T> &v, const R &r)
{
    Vector<N, T> vec(v);

    vec *= r;
    return vec;
}

template<usize N, typename T, typename L>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator*(const L &l, const Vector<N, T> &v)
{
    return v * l;
}

template<usize N, typename T>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator*(Vector<N, T> a, const Vector<N, T> &b)
{
    a *= b;
    return a;
}

template<usize N, typename T, typename R>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator-(const Vector<N, T> &v, const R &r)
{

    Vector<N, T> vec(v);
    vec -= r;
    return vec;
}

template<usize N, typename T, typename L>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator-(const L &l, const Vector<N, T> &v)
{
    return -v + l;
}

template<usize N, typename T>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator-(Vector<N, T> a, const Vector<N, T> &b)
{
    a -= b;
    return a;
}

template<usize N, typename T, typename R>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator/(const Vector<N, T> &v, const R &r)
{
    Vector<N, T> vec(v);
    vec /= r;
    return vec;
}

template<usize N, typename T, typename L>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator/(const L &l, const Vector<N, T> &v)
{
    Vector<N, T> vec(v);
    return vec / v;
}

template<usize N, typename T>
VKM_API VK_NODISCARD VK_INLINE constexpr auto operator/(Vector<N, T> a, const Vector<N, T> &b)
{
    a /= b;
    return a;
}

}// namespace vk::maths
