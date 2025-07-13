#pragma once

#include <Lib/Maths/Vector.hpp>

#include <VK/Types.hpp>

namespace vk::maths {

template<usize N, usize M, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_NOT_NULL(M) && VK_MATHS_ASSERT_NUMBERS(T))

VKM_API struct Matrix {

    public:
        VK_MATHS_MAT_TEMPLATE(N, M, T)

        VK_NODISCARD VK_INLINE VK_CONSTEXPR Matrix() = default;

        VK_NODISCARD VK_INLINE VK_CONSTEXPR explicit Matrix(const T &value)
        {
            std::fill(_data, _data + (N * M), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N * M && (VK_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VK_NODISCARD VK_INLINE VK_CONSTEXPR explicit Matrix(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires(VK_MATHS_ASSERT_CONVERTIBLE(U, T))
        VK_NODISCARD VK_INLINE VK_CONSTEXPR explicit Matrix(const Matrix<N, M, U> &other)
        {
            for (size_type i = 0; i < N * M; ++i) {
                _data[i] = static_cast<T>(other._data[i]);
            }
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR T &operator[](size_type index)
        {
            return _data[index];
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR const T &operator[](size_type index) const
        {
            return _data[index];
        }

        VK_NODISCARD VK_INLINE VK_CONSTEXPR T *data()
        {
            return _data;
        }

    private:
        T _data[N * M] = {0};
};

}// namespace vk::maths
