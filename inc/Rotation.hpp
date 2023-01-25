#ifndef ROTATION_HPP
#define ROTATION_HPP

#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"

const float DEG_2_RAD = M_PI / 180.0;

Matrix CreateRotationMatrix(float yaw, float pitch, float roll)
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

    Matrix matrix;
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

Vector DotProduct(Matrix m, Vector v)
{
    Vector result;

    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;

    return result;
}

#endif // ROTATION_HPP