#include <criterion/criterion.h>

#include <Lib/Maths/Vector.hpp>
#include <criterion/internal/test.h>

Test(Vector, test_maths_vector)
{
    vk::maths::Vector<3, f32> vec1(1.0f, 2.0f, 3.0f);
    vk::maths::Vector<2, u32> vec2(4u, 5u);
    vk::maths::Vector<1, i64> vec3(6ll);

    cr_assert_eq(vec1[0], 1.0f);
    cr_assert_eq(vec1[1], 2.0f);
    cr_assert_eq(vec1[2], 3.0f);
    cr_assert_eq(vec2[0], 4u);
    cr_assert_eq(vec2[1], 5u);
    cr_assert_eq(vec3[0], 6ll);
    cr_assert_eq(vec1.length(), std::sqrt(14.0f));
}

Test(Vector, test_maths_vector_normalization)
{
    vk::maths::Vector<3, f32> vec(3.0f, 4.0f, 0.0f);
    vk::maths::Vector<3, f32> vec3 = vec.normalized();

    vec.normalize();
    cr_assert_eq(vec[0], 0.6f);
    cr_assert_eq(vec[1], 0.8f);
    cr_assert_eq(vec[2], 0.0f);
    cr_assert_eq(vec3[0], 0.6f);
    cr_assert_eq(vec3[1], 0.8f);
    cr_assert_eq(vec3[2], 0.0f);
}

Test(Vector, test_maths_vector_operations)
{
    const vk::maths::Vector<3, f32> vec1(1.0f, 2.0f, 3.0f);
    const vk::maths::Vector<3, f32> vec2(4.0f, 5.0f, 6.0f);
    const auto sum = vec1 + vec2;
    const auto diff = vec1 - vec2;
    const auto scaled = vec1 * 2.0f;

    cr_assert_eq(sum[0], 5.0f);
    cr_assert_eq(sum[1], 7.0f);
    cr_assert_eq(sum[2], 9.0f);
    cr_assert_eq(diff[0], -3.0f);
    cr_assert_eq(diff[1], -3.0f);
    cr_assert_eq(diff[2], -3.0f);
    cr_assert_eq(scaled[0], 2.0f);
    cr_assert_eq(scaled[1], 4.0f);
    cr_assert_eq(scaled[2], 6.0f);
}

Test(Vector, test_maths_vector_cross_product)
{
    const vk::maths::Vector<3, f32> vec1(1.0f, 2.0f, 3.0f);
    const vk::maths::Vector<3, f32> vec2(4.0f, 5.0f, 6.0f);
    const auto cross = vec1.cross(vec2);

    cr_assert_eq(cross[0], -3.0f);
    cr_assert_eq(cross[1], 6.0f);
    cr_assert_eq(cross[2], -3.0f);
}

Test(Vector, test_maths_vector_dot_product)
{
    const vk::maths::Vector<3, f32> vec1(1.0f, 2.0f, 3.0f);
    const vk::maths::Vector<3, f32> vec2(4.0f, 5.0f, 6.0f);
    const auto dot = vec1.dot(vec2);

    cr_assert_eq(dot, 32.0f);
}
