#include <criterion/criterion.h>

#include <Lib/Maths/Matrix.hpp>

Test(Matrix, test_maths_matrix)
{
    vk::maths::Matrix<2, 2, f32> mat1(1.0f, 2.0f, 3.0f, 4.0f);
    vk::maths::Matrix<3, 3, u32> mat2(5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u);
    vk::maths::Matrix<1, 1, i64> mat3(14ll);

    cr_assert_eq(mat1[0], 1.0f);
    cr_assert_eq(mat1[1], 2.0f);
    cr_assert_eq(mat1[2], 3.0f);
    cr_assert_eq(mat1[3], 4.0f);
    cr_assert_eq(mat2[0], 5u);
    cr_assert_eq(mat2[1], 6u);
    cr_assert_eq(mat2[2], 7u);
    cr_assert_eq(mat2[3], 8u);
    cr_assert_eq(mat2[4], 9u);
    cr_assert_eq(mat2[5], 10u);
    cr_assert_eq(mat2[6], 11u);
    cr_assert_eq(mat2[7], 12u);
    cr_assert_eq(mat2[8], 13u);
    cr_assert_eq(mat3[0], 14ll);
}
