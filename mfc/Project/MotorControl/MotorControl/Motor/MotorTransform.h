#pragma once

struct Matrix3x3 {
	double m[3][3];
	
    Matrix3x3 operator*(const Matrix3x3& other) const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];
        return result;
    }

    POINT transformPoint(double x, double y) const {
        POINT p;
        p.x = static_cast<LONG>(m[0][0] * x + m[0][1] * y + m[0][2]);
        p.y = static_cast<LONG>(m[1][0] * x + m[1][1] * y + m[1][2]);
        return p;
    }
};

class MotorTransform {
public:
    Matrix3x3 translation(double x, double y);
    Matrix3x3 rotation(double theta);
    Matrix3x3 scaling(double sx, double sy);
    Matrix3x3 makeTransform(double x, double y, double theta, double sx, double sy);
};