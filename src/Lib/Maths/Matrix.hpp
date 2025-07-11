#pragma once

#include <Lib/Maths/Vector.hpp>

#include <VK/Types.hpp>

namespace vk::maths {

template<usize N, usize M, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_NOT_NULL(M) && VK_MATHS_ASSERT_NUMBERS(T))

struct Matrix {

    public:
        VK_MATHS_MAT_TEMPLATE(N, M, T)

        VKM_API VK_NODISCARD VK_INLINE constexpr Matrix() = default;

        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Matrix(const T &value)
        {
            std::fill(_data, _data + (N * M), value);
        }

        template<typename... Args>
            requires(sizeof...(Args) == N * M && (VK_MATHS_ASSERT_CONVERTIBLE(Args, T) && ...))
        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Matrix(Args &&...args) : _data{static_cast<T>(args)...}
        {
            /* __conversion__ */
        }

        template<typename U>
            requires(VK_MATHS_ASSERT_CONVERTIBLE(U, T))
        VKM_API VK_NODISCARD VK_INLINE constexpr explicit Matrix(const Matrix<N, M, U> &other)
        {
            for (size_type i = 0; i < N * M; ++i) {
                _data[i] = static_cast<T>(other._data[i]);
            }
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T &operator[](size_type index)
        {
            return _data[index];
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr const T &operator[](size_type index) const
        {
            return _data[index];
        }

        VKM_API VK_NODISCARD VK_INLINE constexpr T *data()
        {
            return _data;
        }

    private:
        T _data[N * M] = {0};
};

}// namespace vk::maths
