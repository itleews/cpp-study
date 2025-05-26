#include "pch.h"
#include <cmath>
#include "MotorTransform.h"

Matrix3x3 MotorTransform::translation(double x, double y) {
    return Matrix3x3{ {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::rotation(double theta) {
    return Matrix3x3{ {
        {cos(theta), -sin(theta), 0},
        {sin(theta),  cos(theta), 0},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::scaling(double sx, double sy) {
    return Matrix3x3{ {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    } };
}

Matrix3x3 MotorTransform::makeTransform(double x, double y, double theta, double sx, double sy) {
    return translation(x, y) * rotation(theta) * scaling(sx, sy);
}