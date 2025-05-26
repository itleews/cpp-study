#pragma once

#include <vector>

class Motor {
public:
    int m_id, motorSpeed;
    bool isX, isRotating = false;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	double rotationAngle = 0.0; // 모터의 회전 각도

	Motor* parentMotor = nullptr; // 부모 모터 포인터
    CPoint motorOffset;
    std::vector<Motor*> children;

	Motor(); // 기본 생성자
	~Motor(); // 소멸자
    Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size, int speed);
    Motor(int id, CPoint motor, CSize size, double angle);
    void MoveMotor(double deltaTime);
    void MoveByParentDelta(const CPoint& delta);
};