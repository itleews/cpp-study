#pragma once

#include <vector>

class MotorAxis {
public:
    int id;
    CPoint position;
    std::vector<MotorAxis*> children;

    MotorAxis(int _id, CPoint _pos);
    void MoveTo(CPoint dest);
};