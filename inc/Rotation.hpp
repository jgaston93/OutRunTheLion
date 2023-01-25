#ifndef ROTATION_HPP
#define ROTATION_HPP

#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"

const float DEG_2_RAD = M_PI / 180.0;

Matrix3x3 CreateRotationMatrix(float yaw, float pitch, float roll)
{
    float yaw_rad = yaw * DEG_2_RAD;
    float pitch_rad = pitch * DEG_2_RAD;
    float roll_rad = roll * DEG_2_RAD;

    float cos_yaw = cos(yaw_rad);
    float sin_yaw = sin(yaw_rad);
    float cos_pitch = cos(pitch_rad);
    float sin_pitch = sin(pitch_rad);
    float cos_roll = cos(roll_rad);
    float sin_roll = cos(roll_rad);

    Matrix3x3 matrix;
    matrix.m[0][0] = cos_yaw * cos_pitch;
    matrix.m[0][1] = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
    matrix.m[0][2] = cos_yaw * sin_pitch * cos_roll + sin_pitch * sin_roll;
    matrix.m[1][0] = sin_yaw * cos_pitch;
    matrix.m[1][1] = sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll;
    matrix.m[1][2] = sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll;
    matrix.m[2][0] = -sin_pitch;
    matrix.m[2][1] = cos_yaw * sin_roll;
    matrix.m[2][2] = cos_pitch * cos_roll;

    return matrix;
}

Vector3 DotProduct(Matrix3x3 m, Vector3 v)
{
    Vector3 result;

    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;

    return result;
}

Matrix4x4 Multiply(Matrix4x4 m_1, Matrix4x4 m_2)
{
    Matrix4x4 result;

    result.m[0][0] = m_1.m[0][0] * m_2.m[0][0] + m_1.m[0][1] * m_2.m[1][0] + m_1.m[0][2] * m_2.m[2][0] + m_1.m[0][3] * m_2.m[3][0];
    result.m[0][1] = m_1.m[0][0] * m_2.m[0][1] + m_1.m[0][1] * m_2.m[1][1] + m_1.m[0][2] * m_2.m[2][1] + m_1.m[0][3] * m_2.m[3][1];
    result.m[0][2] = m_1.m[0][0] * m_2.m[0][2] + m_1.m[0][1] * m_2.m[1][2] + m_1.m[0][2] * m_2.m[2][2] + m_1.m[0][3] * m_2.m[3][2];
    result.m[0][3] = m_1.m[0][0] * m_2.m[0][3] + m_1.m[0][1] * m_2.m[1][3] + m_1.m[0][2] * m_2.m[2][3] + m_1.m[0][3] * m_2.m[3][3];
    
    result.m[1][0] = m_1.m[1][0] * m_2.m[0][0] + m_1.m[1][1] * m_2.m[1][0] + m_1.m[1][2] * m_2.m[2][0] + m_1.m[1][3] * m_2.m[3][0];
    result.m[1][1] = m_1.m[1][0] * m_2.m[0][1] + m_1.m[1][1] * m_2.m[1][1] + m_1.m[1][2] * m_2.m[2][1] + m_1.m[1][3] * m_2.m[3][1];
    result.m[1][2] = m_1.m[1][0] * m_2.m[0][2] + m_1.m[1][1] * m_2.m[1][2] + m_1.m[1][2] * m_2.m[2][2] + m_1.m[1][3] * m_2.m[3][2];
    result.m[1][3] = m_1.m[1][0] * m_2.m[0][3] + m_1.m[1][1] * m_2.m[1][3] + m_1.m[1][2] * m_2.m[2][3] + m_1.m[1][3] * m_2.m[3][3];
    
    result.m[2][0] = m_1.m[2][0] * m_2.m[0][0] + m_1.m[2][1] * m_2.m[1][0] + m_1.m[2][2] * m_2.m[2][0] + m_1.m[2][3] * m_2.m[3][0];
    result.m[2][1] = m_1.m[2][0] * m_2.m[0][1] + m_1.m[2][1] * m_2.m[1][1] + m_1.m[2][2] * m_2.m[2][1] + m_1.m[2][3] * m_2.m[3][1];
    result.m[2][2] = m_1.m[2][0] * m_2.m[0][2] + m_1.m[2][1] * m_2.m[1][2] + m_1.m[2][2] * m_2.m[2][2] + m_1.m[2][3] * m_2.m[3][2];
    result.m[2][3] = m_1.m[2][0] * m_2.m[0][3] + m_1.m[2][1] * m_2.m[1][3] + m_1.m[2][2] * m_2.m[2][3] + m_1.m[2][3] * m_2.m[3][3];
    
    result.m[3][0] = m_1.m[3][0] * m_2.m[0][0] + m_1.m[3][1] * m_2.m[1][0] + m_1.m[3][2] * m_2.m[2][0] + m_1.m[3][3] * m_2.m[3][0];
    result.m[3][1] = m_1.m[3][0] * m_2.m[0][1] + m_1.m[3][1] * m_2.m[1][1] + m_1.m[3][2] * m_2.m[2][1] + m_1.m[3][3] * m_2.m[3][1];
    result.m[3][2] = m_1.m[3][0] * m_2.m[0][2] + m_1.m[3][1] * m_2.m[1][2] + m_1.m[3][2] * m_2.m[2][2] + m_1.m[3][3] * m_2.m[3][2];
    result.m[3][3] = m_1.m[3][0] * m_2.m[0][3] + m_1.m[3][1] * m_2.m[1][3] + m_1.m[3][2] * m_2.m[2][3] + m_1.m[3][3] * m_2.m[3][3];

    return result;
}

Matrix4x4 CreateModelMatrix(Transform transform)
{
    Matrix4x4 translation_matrix;
    memset(&translation_matrix, 0, sizeof(Matrix4x4));
    translation_matrix.m[0][0] = 1;
    translation_matrix.m[0][3] = transform.position.x;
    translation_matrix.m[1][1] = 1;
    translation_matrix.m[1][3] = transform.position.y;
    translation_matrix.m[2][2] = 1;
    translation_matrix.m[2][3] = transform.position.z;
    translation_matrix.m[3][3] = 1;
    
    Matrix4x4 scale_matrix;
    memset(&scale_matrix, 0, sizeof(Matrix4x4));
    scale_matrix.m[0][0] = transform.position.x;
    scale_matrix.m[1][1] = transform.position.y;
    scale_matrix.m[2][2] = transform.position.z;
    scale_matrix.m[3][3] = 1;

    Matrix3x3 rotation_matrix = CreateRotationMatrix(transform.rotation.x,
                                                     transform.rotation.y,
                                                     transform.rotation.z);
    Matrix4x4 rotation_matrix_4x4;

    rotation_matrix_4x4.m[0][0] = rotation_matrix.m[0][0];
    rotation_matrix_4x4.m[0][1] = rotation_matrix.m[0][1];
    rotation_matrix_4x4.m[0][2] = rotation_matrix.m[0][2];
    rotation_matrix_4x4.m[0][3] = 0;
    rotation_matrix_4x4.m[1][0] = rotation_matrix.m[0][0];
    rotation_matrix_4x4.m[1][1] = rotation_matrix.m[0][1];
    rotation_matrix_4x4.m[1][2] = rotation_matrix.m[0][2];
    rotation_matrix_4x4.m[1][3] = 0;
    rotation_matrix_4x4.m[2][0] = rotation_matrix.m[0][0];
    rotation_matrix_4x4.m[2][1] = rotation_matrix.m[0][1];
    rotation_matrix_4x4.m[2][2] = rotation_matrix.m[0][2];
    rotation_matrix_4x4.m[2][3] = 0;
    rotation_matrix_4x4.m[3][0] = rotation_matrix.m[0][0];
    rotation_matrix_4x4.m[3][1] = rotation_matrix.m[0][1];
    rotation_matrix_4x4.m[3][2] = rotation_matrix.m[0][2];
    rotation_matrix_4x4.m[3][3] = 1;

    Matrix4x4 model_matrix = Multiply(Multiply(translation_matrix, rotation_matrix_4x4), scale_matrix);
    return model_matrix;
}

Matrix4x4 CreateViewMatrix(Transform transform)
{
    Matrix3x3 rotation_matrix = CreateRotationMatrix(-transform.rotation.x,
                                                     -transform.rotation.y,
                                                     -transform.rotation.z);
    Matrix4x4 view_matrix;

    view_matrix.m[0][0] = rotation_matrix.m[0][0];
    view_matrix.m[0][1] = rotation_matrix.m[0][1];
    view_matrix.m[0][2] = rotation_matrix.m[0][2];
    view_matrix.m[0][3] = -transform.position.x;
    view_matrix.m[1][0] = rotation_matrix.m[0][0];
    view_matrix.m[1][1] = rotation_matrix.m[0][1];
    view_matrix.m[1][2] = rotation_matrix.m[0][2];
    view_matrix.m[1][3] = -transform.position.y;
    view_matrix.m[2][0] = rotation_matrix.m[0][0];
    view_matrix.m[2][1] = rotation_matrix.m[0][1];
    view_matrix.m[2][2] = rotation_matrix.m[0][2];
    view_matrix.m[2][3] = -transform.position.z;
    view_matrix.m[3][0] = rotation_matrix.m[0][0];
    view_matrix.m[3][1] = rotation_matrix.m[0][1];
    view_matrix.m[3][2] = rotation_matrix.m[0][2];
    view_matrix.m[3][3] = 1;

    return view_matrix;
}

#endif // ROTATION_HPP