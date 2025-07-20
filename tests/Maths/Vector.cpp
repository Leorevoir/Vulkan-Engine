#include <criterion/criterion.h>

#include <VKE/Maths/Vector.hpp>

Test(Vector, test_maths_vector)
{
    vke::maths::Vector<3, f32> vec1(1.f, 2.f, 3.f);
    vke::maths::Vector<2, u32> vec2(4u, 5u);
    vke::maths::Vector<1, i64> vec3(6ll);

    cr_assert_eq(vec1[0], 1.f);
    cr_assert_eq(vec1[1], 2.f);
    cr_assert_eq(vec1[2], 3.f);
    cr_assert_eq(vec2[0], 4u);
    cr_assert_eq(vec2[1], 5u);
    cr_assert_eq(vec3[0], 6ll);
    cr_assert_eq(vec1.length(), std::sqrt(14.f));
}

Test(Vector, test_maths_vector_normalization)
{
    vke::maths::Vector<3, f32> vec(3.f, 4.f, 0.f);
    vke::maths::Vector<3, f32> vec3 = vec.normalized();

    vec.normalize();
    cr_assert_eq(vec[0], 0.6f);
    cr_assert_eq(vec[1], 0.8f);
    cr_assert_eq(vec[2], 0.f);
    cr_assert_eq(vec3[0], 0.6f);
    cr_assert_eq(vec3[1], 0.8f);
    cr_assert_eq(vec3[2], 0.f);
}

Test(Vector, test_maths_vector_operations)
{
    const vke::maths::Vector<3, f32> vec1(1.f, 2.f, 3.f);
    const vke::maths::Vector<3, f32> vec2(4.f, 5.f, 6.f);
    const auto sum = vec1 + vec2;
    const auto diff = vec1 - vec2;
    const auto scaled = vec1 * 2.f;

    cr_assert_eq(sum[0], 5.f);
    cr_assert_eq(sum[1], 7.f);
    cr_assert_eq(sum[2], 9.f);
    cr_assert_eq(diff[0], -3.f);
    cr_assert_eq(diff[1], -3.f);
    cr_assert_eq(diff[2], -3.f);
    cr_assert_eq(scaled[0], 2.f);
    cr_assert_eq(scaled[1], 4.f);
    cr_assert_eq(scaled[2], 6.f);
}

Test(Vector, test_maths_vector_cross_product)
{
    const vke::maths::Vector<3, f32> vec1(1.f, 2.f, 3.f);
    const vke::maths::Vector<3, f32> vec2(4.f, 5.f, 6.f);
    const auto cross = vec1.cross(vec2);

    cr_assert_eq(cross[0], -3.f);
    cr_assert_eq(cross[1], 6.f);
    cr_assert_eq(cross[2], -3.f);
}

Test(Vector, test_maths_vector_dot_product)
{
    const vke::maths::Vector<3, f32> vec1(1.f, 2.f, 3.f);
    const vke::maths::Vector<3, f32> vec2(4.f, 5.f, 6.f);
    const auto dot = vec1.dot(vec2);

    cr_assert_eq(dot, 32.f);
}

Test(Vector, test_maths_vector_member_access)
{
    vke::maths::Vector3f vec3(1.f, 2.f, 3.f);
    vke::maths::Vector4f vec4(4.f, 5.f, 6.f, 7.f);

    cr_assert_eq(vec3.x, 1.f);
    cr_assert_eq(vec3.y, 2.f);
    cr_assert_eq(vec3.z, 3.f);
    cr_assert_eq(vec4.x, 4.f);
    cr_assert_eq(vec4.y, 5.f);
    cr_assert_eq(vec4.z, 6.f);
    cr_assert_eq(vec4.w, 7.f);

    vec4.w = vec4.x + vec4.y + vec4.z;

    cr_assert_eq(vec4.r, vec4.x);
    cr_assert_eq(vec4.g, vec4.y);
    cr_assert_eq(vec4.b, vec4.z);
    cr_assert_eq(vec4.a, vec4.w);

    vec3.x = 10.f;

    cr_assert_eq(vec3.s, vec3.x);

    vec3._data[0] = 20.f;

    cr_assert_eq(vec3.x, 20.f);
}
