#pragma once

#include <vector>

class Motor {
public:
    int m_id, motorSpeed;
    bool isX;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	Motor* parentMotor = nullptr; // �θ� ���� ������
    CPoint motorOffset;
    std::vector<Motor*> children;

    Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size, int speed);
    void MoveMotor(double deltaTime);
    void MoveByParentDelta(const CPoint& delta);
};