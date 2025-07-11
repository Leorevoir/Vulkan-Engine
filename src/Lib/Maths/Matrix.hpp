#pragma once

#include <Lib/Maths/Base.hpp>
#include <VK/Types.hpp>

namespace vk::maths {

template<usize N, usize M, typename T>
    requires(VK_MATHS_NOT_NULL(N) && VK_MATHS_NOT_NULL(M) && VK_MATHS_ASSERT_NUMBERS(T))

struct Matrix {

    public:
        VK_MATHS_MAT_TEMPLATE(N, M, T)

    private:
};

}// namespace vk::maths
