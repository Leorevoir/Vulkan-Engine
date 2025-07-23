#include <VKE/Maths/Matrix.hpp>

VKE_NODISCARD vke::maths::Matrix4f vke::maths::operator*(const Matrix4f &lhs, const Matrix4f &rhs)
{
    Matrix4f result(0.f);

    for (u32 col = 0; col < 4; ++col) {

        for (u32 row = 0; row < 4; ++row) {

            for (u32 k = 0; k < 4; ++k) {
                result[col][row] += lhs[k][row] * rhs[col][k];
            }
        }
    }

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::perspective(const f32 fovy, const f32 aspect, const f32 near, const f32 far)
{
    const f32 tan_half_fovy = std::tan(fovy / 2.f);

    Matrix4f result(0.f);

    result[0][0] = 1.f / (aspect * tan_half_fovy);
    result[1][1] = 1.f / tan_half_fovy;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.f;
    result[3][2] = -(2.f * far * near) / (far - near);

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::translate(const Matrix4f &mat, const Vector3f &translation)
{
    Matrix4f result = mat;

    result[3][0] += translation.x;
    result[3][1] += translation.y;
    result[3][2] += translation.z;

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::rotate(f32 angle_radians, const Vector3f &axis)
{
    const Vector3f new_axis = axis.normalized();
    const f32 c = std::cos(angle_radians);
    const f32 s = std::sin(angle_radians);
    const f32 t = 1.f - c;
    const f32 x = new_axis.x, y = new_axis.y, z = new_axis.z;
    Matrix4f result(1.f);

    result[0][0] = t * x * x + c;
    result[0][1] = t * x * y + s * z;
    result[0][2] = t * x * z - s * y;

    result[1][0] = t * x * y - s * z;
    result[1][1] = t * y * y + c;
    result[1][2] = t * y * z + s * x;

    result[2][0] = t * x * z + s * y;
    result[2][1] = t * y * z - s * x;
    result[2][2] = t * z * z + c;

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::rotate(const Matrix4f &mat, f32 angle_radians, const Vector3f &axis)
{
    return mat * rotate(angle_radians, axis);
}

VKE_NODISCARD vke::maths::Matrix3f vke::maths::inverse(const Matrix3f &m)
{
    const f32 a = m[0][0], b = m[0][1], c = m[0][2];
    const f32 d = m[1][0], e = m[1][1], f = m[1][2];
    const f32 g = m[2][0], h = m[2][1], i = m[2][2];
    const f32 det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    if (std::abs(det) < VKEM_EPSILON) {
        return Matrix3f(0.f);
    }

    const f32 inv_det = 1.f / det;
    Matrix3f result;

    result[0][0] = (e * i - f * h) * inv_det;
    result[0][1] = (c * h - b * i) * inv_det;
    result[0][2] = (b * f - c * e) * inv_det;
    result[1][0] = (f * g - d * i) * inv_det;
    result[1][1] = (a * i - c * g) * inv_det;
    result[1][2] = (c * d - a * f) * inv_det;
    result[2][0] = (d * h - e * g) * inv_det;
    result[2][1] = (b * g - a * h) * inv_det;
    result[2][2] = (a * e - b * d) * inv_det;

    return result;
}

VKE_NODISCARD vke::maths::Matrix3f vke::maths::transpose(const Matrix3f &m)
{
    Matrix3f result;

    for (usize col = 0; col < 3; ++col) {
        for (usize row = 0; row < 3; ++row) {
            result[col][row] = m[row][col];
        }
    }

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::inverseTranspose(const Matrix4f &m)
{
    Matrix3f upper_left;

    for (usize col = 0; col < 3; ++col) {
        for (usize row = 0; row < 3; ++row) {
            upper_left[col][row] = m[col][row];
        }
    }

    const Matrix3f inverse_transpose = transpose(inverse(upper_left));
    Matrix4f result(1.f);

    for (usize col = 0; col < 3; ++col) {
        for (usize row = 0; row < 3; ++row) {
            result[col][row] = inverse_transpose[col][row];
        }
    }

    return result;
}
