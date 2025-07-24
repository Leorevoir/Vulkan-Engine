#include <criterion/criterion.h>
#include <iostream>

#include <VKE/Maths/Maths.hpp>
#include <VKE/Maths/Matrix.hpp>

/**
* direct comparaison with glm
*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
* helpers
*/

template<typename T, typename U>
static inline void _assert_equal(const T &mat1, const U &mat2)
{
    bool is_false = false;
    f32 a, b;

    for (u32 i = 0; i < mat1.size; ++i) {
        for (u32 j = 0; j < mat1.size; ++j) {

            if (mat1[i][j] != mat2[i][j]) {
                is_false = true;
                a = mat1[i][j];
                b = mat2[i][j];
                break;
            }
        }
    }
    if (is_false) {
        for (u32 i = 0; i < mat1.size; ++i) {
            for (u32 j = 0; j < mat1.size; ++j) {
                std::cout << "mat1[" << i << "][" << j << "] = " << mat1[i][j] << std::endl;
                std::cout << "mat2[" << i << "][" << j << "] = " << mat2[i][j] << std::endl;
            }
        }
        cr_assert_eq(a, b);
    }
}
#define assert_equal(mat1, glm_mat1) _assert_equal(mat1, glm_mat1)

/**
* tests
*/

using namespace vke;

Test(Matrix, test_maths_matrix)
{
    const maths::Matrix<2, 2, f32> mat1(1.f, 2.f, 3.f, 4.f);
    const glm::mat2x2 glm_mat1(1.f, 2.f, 3.f, 4.f);

    cr_assert_eq(mat1[0][0], glm_mat1[0][0]);
    cr_assert_eq(mat1[0][1], glm_mat1[0][1]);
    cr_assert_eq(mat1[1][0], glm_mat1[1][0]);
    cr_assert_eq(mat1[1][1], glm_mat1[1][1]);
    cr_assert_eq(mat1.size, glm_mat1.length());
}

Test(Matrix, test_maths_matrix_methods)
{
    maths::Matrix4f mat4(0.411341f);
    glm::mat4 glm_mat4(0.411341f);

    constexpr f32 aspect_ratio = 1.f;

    constexpr f32 fov = 60.f;
    constexpr f32 fov_rad = maths::radians(fov);
    constexpr f32 fov_rad_glm = glm::radians(fov);

    cr_assert_eq(fov_rad, fov_rad_glm);

    const auto projection = maths::perspective(fov_rad, aspect_ratio, 0.001f, 256.0f);
    const auto glm_projection = glm::perspective(fov_rad_glm, aspect_ratio, 0.001f, 256.0f);

    assert_equal(projection, glm_projection);

    const auto translate = maths::translate(maths::Matrix4f::Identity(), maths::Vector3f(0.f, -0.f, -2.f));
    const auto glm_translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -0.f, -2.f));

    assert_equal(translate, glm_translate);

    const auto rotate = maths::rotate(translate, maths::radians(90.f), maths::Vector3f(1.0f, 0.0f, 0.0f));
    const auto glm_rotate = glm::rotate(glm_translate, glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

    assert_equal(rotate, glm_rotate);

    const auto inverse_transpose = maths::inverseTranspose(rotate);
    const auto glm_inverse_transpose = glm::inverseTranspose(glm_rotate);

    cr_assert_eq(inverse_transpose[0][0], glm_inverse_transpose[0][0]);
}
