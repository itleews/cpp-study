#pragma once

#include <vector>

class Motor {
public:
    int m_id;
    bool isX;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	std::vector<Motor*> subMotors; // ���� ���� ����Ʈ
	Motor* parentMotor = nullptr; // �θ� ���� ������

    std::vector<Motor*> children;

    Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size);
    /*void MoveTo(CPoint dest);*/
};