#pragma once

#include <vector>

class MotorAxis {
public:
    int m_id;
    CPoint strPos;
    CPoint endPos;
    std::vector<MotorAxis*> children;

    MotorAxis(int id, CPoint str, CPoint end);
    /*void MoveTo(CPoint dest);*/
};