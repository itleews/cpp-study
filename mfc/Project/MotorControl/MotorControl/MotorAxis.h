#pragma once

#include <vector>

class MotorAxis {
public:
    int m_id;
    bool isX;

    // ³í¸® ÁÂÇ¥ (0.0 ~ 1.0)
    double strXRatio, strYRatio;
    double endXRatio, endYRatio;
    double motorXRatio, motorYRatio;
    CSize motorSize;

    std::vector<MotorAxis*> children;

    MotorAxis(int id, bool isXDirection, double sX, double sY, double eX, double eY, double mX, double mY, CSize size);
    /*void MoveTo(CPoint dest);*/
};