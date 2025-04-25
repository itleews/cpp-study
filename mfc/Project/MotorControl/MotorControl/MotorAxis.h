#pragma once

#include <vector>

class MotorAxis {
public:
    int m_id;
    bool isX;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;

    std::vector<MotorAxis*> children;

    MotorAxis(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size);
    /*void MoveTo(CPoint dest);*/
};