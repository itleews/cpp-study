#pragma once

#include <vector>

class Motor {
public:
    int m_id, motorSpeed;
    bool isX, isRotating = false;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	double rotationAngle = 0.0; // ������ ȸ�� ����

	Motor* parentMotor = nullptr; // �θ� ���� ������
    CPoint motorOffset;
    std::vector<Motor*> children;

	Motor(); // �⺻ ������
	~Motor(); // �Ҹ���
    Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size, int speed);
    Motor(int id, CPoint motor, CSize size, double angle);
    void MoveMotor(double deltaTime);
    void MoveByParentDelta(const CPoint& delta);
};