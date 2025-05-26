#pragma once

#include <vector>

enum AxisType {
    None = 0,
    X = 1,
    Y = 2,
    T = 3,
};


class Motor {
public:
    int m_id, motorSpeed;
    AxisType axis = None;
    bool isRotating = false;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	double rotationAngle = 0.0; // 모터의 회전 각도

	Motor* parentMotor = nullptr; // 부모 모터 포인터
    CPoint motorOffset;
    std::vector<Motor*> children;

	Motor(); // 기본 생성자
	~Motor(); // 소멸자
    Motor(int id, AxisType axis, CPoint str, CPoint end, CPoint motor, CSize size, int speed);
    Motor(int id, CPoint motor, CSize size, double angle);
    void MoveMotor(double deltaTime);
    void MoveByParentDelta(const CPoint& delta);
};