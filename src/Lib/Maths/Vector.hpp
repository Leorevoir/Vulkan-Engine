#pragma once

#include <Lib/Maths/Base.hpp>
#include <VK/Macros.hpp>
#include <VK/Types.hpp>

#include <cmath>
#include <initializer_list>
#include <ostream>

namespace vk::maths {

namespace detail {

/**
 * NOTE: disable pedantic warnings for GCC/Clang to allow anonymous structs
 */

#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#endif

template<usize N, typename T>
struct VectorData {

        /* __data__ */
        T _data[N] = {0};
};

template<typename T>
struct VectorData<2, T> {

        union {
                struct {
                        T x, y;
                };
                struct {
                        T r, g;
                };
                struct {
                        T s, t;
                };
                struct {
                        T width, height;
                };
                T _data[2];
        };
};

template<typename T>
struct VectorData<3, T> {

        union {
                struct {
                        T x, y, z;
                };
                struct {
                        T r, g, b;
                };
                struct {
                        T s, t, p;
                };
                T _data[3];
        };
};

template<typename T>
struct VectorData<4, T> {

        union {
                struct {
                        T x, y, z, w;
                };
                struct {
                        T r, g, b, a;
                };
                struct {
                        T s, t, p, q;
                };
                T _data[2];
        };
};

#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif

}// namespace detail

template<usize N, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_ASSERT_NUMBERS(T))

VKM_API struct Vector : public detail::VectorData<N, T> {

        VK_MATHS_ASSERT_VECTOR_SIZE(N)

    public:
        VK_MATHS_VEC_TEMPLATE(N, T)

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector() = default;

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector(const T &value)
        {
            std::fill(this->_data.begin(), this->_data.end(), value);
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector(const std::initializer_list<T> &il)
        {
            size_type i = 0;

            for (const auto &v : il) {
                if (i < N) {
                    this->_data[i++] = v;
                }
            }

            for (; i < N; ++i) {
                this->_data[i] = T(0);
            }
        }

        template<typename... Args>
            requires(sizeof...(Args) == N && (VK_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector(Args &&...args)
        {
            const T temp[N] = {static_cast<T>(args)...};

            for (usize i = 0; i < N; ++i) {
                this->_data[i] = temp[i];
            }
        }

        template<typename U>
            requires(VK_MATHS_ASSERT_CONVERTIBLE(U, T))
        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector(const Vector<N, U> &other)
        {
            for (size_type i = 0; i < N; ++i) {
                this->_data[i] = static_cast<T>(other[i]);
            }
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector operator+() const
        {
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector operator-() const
        {
            Vector v;

            for (size_type i = 0; i != N; ++i) {
                v[i] = -this->_data[i];
            }
            return v;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator*=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] *= t;
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator/=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] /= t;
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator+=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] += t;
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator-=(const T &t)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] -= t;
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator*=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] *= v[i];
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator/=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] /= v[i];
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator+=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] += v[i];
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator-=(const Vector &v)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] -= v[i];
            }
            return *this;
        }

        VK_INLINE VK_CONSTEXPR Vector &operator=(const T &v)
        {
            for (size_type i = 0; i != N; ++i) {
                this->_data[i] = v;
            }
            return *this;
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR T &operator[](size_type index)
        {
            return this->_data[index];
        }
        VK_NODISCARD VK_INLINE VK_CONSTEXPR const T &operator[](size_type index) const
        {
            return this->_data[index];
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR T dot(const Vector &o) const
        {
            T sum = 0;

            for (size_type i = 0; i != N; ++i) {
                sum += this->_data[i] * o[i];
            }
            return sum;
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector cross(const Vector &o) const
        {
            Vector v;

            for (size_type i = 0; i != N; ++i) {
                v[i] = this->_data[(i + 1) % N] * o[(i + 2) % N] - this->_data[(i + 2) % N] * o[(i + 1) % N];
            }
            return v;
        }

        VK_INLINE VK_CONSTEXPR bool is_zero() const
        {
            for (const auto &value : this->_data) {
                if (value != 0) {
                    return false;
                }
            }
            return true;
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR T length() const
        {
            return std::sqrt(dot(*this));
        }

        VK_INLINE VK_CONSTEXPR void normalize()
        {
            if (!is_zero()) {
                operator*=(1.f / length());
            }
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Vector normalized() const
        {
            Vector v(*this);

            v.normalize();
            return v;
        }
};

template<usize N, typename T>
VK_INLINE VK_CONSTEXPR auto operator+(const Vector<N, T> &v, const T &t)
{
    Vector<N, T> vec(v);

    vec += t;
    return vec;
}

template<usize N, typename T, typename L>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator+(const L &l, const Vector<N, T> &v)
{
    return v + l;
}

template<usize N, typename T>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator+(Vector<N, T> a, const Vector<N, T> &b)
{
    a += b;
    return a;
}

template<usize N, typename T, typename R>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator*(const Vector<N, T> &v, const R &r)
{
    Vector<N, T> vec(v);

    vec *= r;
    return vec;
}

template<usize N, typename T, typename L>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator*(const L &l, const Vector<N, T> &v)
{
    return v * l;
}

template<usize N, typename T>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator*(Vector<N, T> a, const Vector<N, T> &b)
{
    a *= b;
    return a;
}

template<usize N, typename T, typename R>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator-(const Vector<N, T> &v, const R &r)
{
    Vector<N, T> vec(v);

    vec -= r;
    return vec;
}

template<usize N, typename T, typename L>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator-(const L &l, const Vector<N, T> &v)
{
    return -v + l;
}

template<usize N, typename T>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator-(Vector<N, T> a, const Vector<N, T> &b)
{
    a -= b;
    return a;
}

template<usize N, typename T, typename R>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator/(const Vector<N, T> &v, const R &r)
{
    Vector<N, T> vec(v);

    vec /= r;
    return vec;
}

template<usize N, typename T, typename L>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator/(const L &l, const Vector<N, T> &v)
{
    Vector<N, T> vec(v);

    return vec / v;
}

template<usize N, typename T>
VK_NODISCARD VK_INLINE VK_CONSTEXPR auto operator/(Vector<N, T> a, const Vector<N, T> &b)
{
    a /= b;
    return a;
}

template<usize N, typename T>
VK_NODISCARD VK_INLINE VK_CONSTEXPR std::ostream &operator<<(std::ostream &os, const Vector<N, T> &v)
{
    os << "Vector<" << N << ", " << typeid(T).name() << ">(";
    for (typename Vector<N, T>::size_type i = 0; i < N; ++i) {
        os << v[i];
        if (i < N - 1) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}

template<typename T>
using Vector2 = Vector<2, T>;
template<typename T>
using Vector3 = Vector<3, T>;
template<typename T>
using Vector4 = Vector<4, T>;

using Vector2f = Vector2<f32>;
using Vector3f = Vector3<f32>;
using Vector4f = Vector4<f32>;

using Vector2u = Vector2<u32>;
using Vector2i = Vector2<i32>;

}// namespace vk::maths
