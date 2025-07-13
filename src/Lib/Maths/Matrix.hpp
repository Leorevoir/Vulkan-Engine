#pragma once

#include <Lib/Maths/Vector.hpp>

#include <VKE/Types.hpp>

namespace vke::maths {

template<usize N, usize M, typename T>
    requires(VKE_MATHS_NOT_NULL(N) && VKE_MATHS_NOT_NULL(M) && VKE_MATHS_ASSERT_NUMBERS(T))

VKEM_API struct Matrix {

    public:
        VKE_MATHS_MAT_TEMPLATE(N, M, T)

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR Matrix() = default;

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(const T &value)
        {
            std::fill(_data, _data + (N * M), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N * M && (VKE_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires(VKE_MATHS_ASSERT_CONVERTIBLE(U, T))
        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR explicit Matrix(const Matrix<N, M, U> &other)
        {
            for (size_type i = 0; i < N * M; ++i) {
                _data[i] = static_cast<T>(other._data[i]);
            }
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR T &operator[](size_type index)
        {
            return _data[index];
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR const T &operator[](size_type index) const
        {
            return _data[index];
        }

        VKE_NODISCARD VKE_INLINE VKE_CONSTEXPR T *data()
        {
            return _data;
        }

    private:
        T _data[N * M] = {0};
};

}// namespace vke::maths
