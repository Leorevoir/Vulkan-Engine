#include <VKE/Maths/Matrix.hpp>

VKE_NODISCARD vke::maths::Matrix4f vke::maths::operator*(const Matrix4f &lhs, const Matrix4f &rhs)
{
    Matrix4f result(0.f);

    for (u32 row = 0; row < 4; ++row) {
        for (u32 col = 0; col < 4; ++col) {

            for (u32 k = 0; k < 4; ++k) {
                result[col * 4 + row] += lhs[k * 4 + row] * rhs[col * 4 + k];
            }
        }
    }

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::perspective(const f32 fovy, const f32 aspect, const f32 near, const f32 far)
{
    const f32 tan_half_fovy = std::tan(fovy / 2.f);

    Matrix4f result(0.f);

    result[0] = 1.f / (aspect * tan_half_fovy);
    result[5] = 1.f / (tan_half_fovy);
    result[10] = -(far + near) / (far - near);
    result[11] = -1.f;
    result[14] = -(2.f * far * near) / (far - near);
    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::translate(const Matrix4f &mat, const Vector3f &translation)
{
    Matrix4f result = mat;

    result[12] += translation.x;
    result[13] += translation.y;
    result[14] += translation.z;

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

    result[0] = t * x * x + c;
    result[1] = t * x * y + s * z;
    result[2] = t * x * z - s * y;

    result[4] = t * x * y - s * z;
    result[5] = t * y * y + c;
    result[6] = t * y * z + s * x;

    result[8] = t * x * z + s * y;
    result[9] = t * y * z - s * x;
    result[10] = t * z * z + c;

    return result;
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::rotate(const Matrix4f &mat, f32 angle_radians, const Vector3f &axis)
{
    return mat * rotate(angle_radians, axis);
}

VKE_NODISCARD vke::maths::Matrix4f vke::maths::inverseTranspose(const Matrix4f &m)
{
    const f32 subfac_00 = m[10] * m[15] - m[14] * m[11];
    const f32 subfac_01 = m[9] * m[15] - m[13] * m[11];
    const f32 subfac_02 = m[9] * m[14] - m[13] * m[10];
    const f32 subfac_03 = m[8] * m[15] - m[12] * m[11];
    const f32 subfac_04 = m[8] * m[14] - m[12] * m[10];
    const f32 subfac_05 = m[8] * m[13] - m[12] * m[9];
    const f32 subfac_06 = m[6] * m[15] - m[14] * m[7];
    const f32 subfac_07 = m[5] * m[15] - m[13] * m[7];
    const f32 subfac_08 = m[5] * m[14] - m[13] * m[6];
    const f32 subfac_09 = m[4] * m[15] - m[12] * m[7];
    const f32 subfac_10 = m[4] * m[14] - m[12] * m[6];
    const f32 subfac_11 = m[4] * m[13] - m[12] * m[5];
    const f32 subfac_12 = m[6] * m[11] - m[10] * m[7];
    const f32 subfac_13 = m[5] * m[11] - m[9] * m[7];
    const f32 subfac_14 = m[5] * m[10] - m[9] * m[6];
    const f32 subfac_15 = m[4] * m[11] - m[8] * m[7];
    const f32 subfac_16 = m[4] * m[10] - m[8] * m[6];
    const f32 subfac_17 = m[4] * m[9] - m[8] * m[5];

    Matrix4f inverse;

    inverse[0] = +(m[5] * subfac_00 - m[6] * subfac_01 + m[7] * subfac_02);
    inverse[1] = -(m[4] * subfac_00 - m[6] * subfac_03 + m[7] * subfac_04);
    inverse[2] = +(m[4] * subfac_01 - m[5] * subfac_03 + m[7] * subfac_05);
    inverse[3] = -(m[4] * subfac_02 - m[5] * subfac_04 + m[6] * subfac_05);

    inverse[4] = -(m[1] * subfac_00 - m[2] * subfac_01 + m[3] * subfac_02);
    inverse[5] = +(m[0] * subfac_00 - m[2] * subfac_03 + m[3] * subfac_04);
    inverse[6] = -(m[0] * subfac_01 - m[1] * subfac_03 + m[3] * subfac_05);
    inverse[7] = +(m[0] * subfac_02 - m[1] * subfac_04 + m[2] * subfac_05);

    inverse[8] = +(m[1] * subfac_06 - m[2] * subfac_07 + m[3] * subfac_08);
    inverse[9] = -(m[0] * subfac_06 - m[2] * subfac_09 + m[3] * subfac_10);
    inverse[10] = +(m[0] * subfac_07 - m[1] * subfac_09 + m[3] * subfac_11);
    inverse[11] = -(m[0] * subfac_08 - m[1] * subfac_10 + m[2] * subfac_11);

    inverse[12] = -(m[1] * subfac_12 - m[2] * subfac_13 + m[3] * subfac_14);
    inverse[13] = +(m[0] * subfac_12 - m[2] * subfac_15 + m[3] * subfac_16);
    inverse[14] = -(m[0] * subfac_13 - m[1] * subfac_15 + m[3] * subfac_17);
    inverse[15] = +(m[0] * subfac_14 - m[1] * subfac_16 + m[2] * subfac_17);

    const f32 det = m[0] * inverse[0] + m[1] * inverse[1] + m[2] * inverse[2] + m[3] * inverse[3];

    if (std::abs(det) < VKEM_EPSILON) {
        return Matrix4f(1.0f);
    }

    const f32 inv_det = 1.0f / det;

    for (u32 i = 0; i < 16; ++i) {
        inverse[i] *= inv_det;
    }

    /** m[row][col] <-> m[col][row] */
    Matrix4f result;

    for (u32 row = 0; row < 4; ++row) {
        for (u32 col = 0; col < 4; ++col) {
            result[col * 4 + row] = inverse[row * 4 + col];
        }
    }

    return result;
}
