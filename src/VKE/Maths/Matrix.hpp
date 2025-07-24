#pragma once

#include <VKE/Maths/Vector.hpp>

#include <VKE/Types.hpp>

namespace vke::maths {

template<usize N, usize M, typename T>
    requires(VKE_MATHS_NOT_NULL(N) && VKE_MATHS_NOT_NULL(M) && VKE_MATHS_ASSERT_NUMBERS(T))

struct VKEM_API Matrix {

    public:
        VKE_MATHS_MAT_TEMPLATE(N, M, T)

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR Matrix() = default;

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(const T &value)
        {
            /** @brief fill each column with 0 */
            for (size_type i = 0; i < M; ++i) {
                _columns[i] = Column(T(0));
            }

            /** @brief fill the diagonal witht the given value */
            for (size_type i = 0; i < __min_dim; ++i) {
                _columns[i][i] = value;
            }
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR static Matrix Identity()
        {
            Matrix result(T(0));

            for (size_type i = 0; i < __min_dim; ++i) {
                result[i][i] = T(1);
            }
            return result;
        }

        template<typename... Args>
            requires(sizeof...(Args) == N * M && (VKE_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(Args &&...args)
        {
            T values[N * M] = {static_cast<T>(args)...};
            for (size_type i = 0; i < M; ++i) {

                for (size_type j = 0; j < N; ++j) {
                    _columns[i][j] = values[i * N + j];
                }
            }
        }

        template<typename U>
            requires(VKE_MATHS_ASSERT_CONVERTIBLE(U, T))
        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(const Matrix<N, M, U> &other)
        {
            for (size_type i = 0; i < M; ++i) {

                for (size_type j = 0; j < N; ++j) {
                    _columns[i][j] = static_cast<T>(other[i][j]);
                }
            }
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR Column &operator[](size_type index)
        {
            return _columns[index];
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR const Column &operator[](size_type index) const
        {
            return _columns[index];
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR T *data()
        {
            return reinterpret_cast<T *>(_columns);
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR const T *data() const
        {
            return reinterpret_cast<const T *>(_columns);
        }

    private:
        static constexpr size_type __min_dim = (N < M) ? N : M;
        Column _columns[M] = {};
};

template<typename T>
using Matrix2x2 = Matrix<2, 2, T>;
template<typename T>
using Matrix3x3 = Matrix<3, 3, T>;
template<typename T>
using Matrix4x4 = Matrix<4, 4, T>;

using Matrix2f = Matrix2x2<f32>;
using Matrix3f = Matrix3x3<f32>;
using Matrix4f = Matrix4x4<f32>;

VKE_NODISCARD Matrix4f operator*(const Matrix4f &lhs, const Matrix4f &rhs);

VKE_NODISCARD Matrix4f perspective(const f32 fovy, const f32 aspect, const f32 near, const f32 far);
VKE_NODISCARD Matrix4f translate(const Matrix4f &mat, const Vector3f &translation);

VKE_NODISCARD Matrix4f rotate(f32 angle_radians, const Vector3f &axis);
VKE_NODISCARD Matrix4f rotate(const Matrix4f &mat, f32 angle_radians, const Vector3f &axis);

VKE_NODISCARD Matrix3f inverse(const Matrix3f &m);
VKE_NODISCARD Matrix3f transpose(const Matrix3f &m);
VKE_NODISCARD Matrix4f inverseTranspose(const Matrix4f &m);

}// namespace vke::maths
