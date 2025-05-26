#include "pch.h"
#include "MotorAxis.h"

Motor::Motor()
    : m_id(0), isX(true), isRotating(false), strPos(0, 0), endPos(0, 0), motorPos(0, 0),
    motorSize(0, 0), motorSpeed(0) {}

Motor::~Motor()
{
    for (Motor* child : children) {
        delete child;
    }
    children.clear();
}

Motor::Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size, int speed)
	: m_id(id), isX(isXDirection), isRotating(false),
	strPos(str), endPos(end), motorPos(motor),
	motorSize(size), motorSpeed(speed) {}

Motor::Motor(int id, CPoint motor, CSize size, double angle)
    : m_id(id), motorPos(motor), motorSize(size), rotationAngle(angle),
    isX(false), isRotating(true), motorSpeed(0), strPos(motor), endPos(motor) {}

void Motor::MoveMotor(double deltaTime)
{
    CPoint prevMotorPos = motorPos;

    // 자기 자신만 이동
    if (isX)
    {
        if (motorPos.x + motorSize.cx > endPos.x || motorPos.x - motorSize.cx < strPos.x)
            motorSpeed = -motorSpeed;

        motorPos.x += static_cast<int>(motorSpeed * deltaTime);
    }
    else
    {
        if (motorPos.y + motorSize.cy > endPos.y || motorPos.y - motorSize.cy < strPos.y)
            motorSpeed = -motorSpeed;

        motorPos.y += static_cast<int>(motorSpeed * deltaTime);
    }

    CPoint delta = motorPos - prevMotorPos;

    // delta를 자식에게만 전달
    for (auto child : children)
    {
        child->MoveByParentDelta(delta);
        child->MoveMotor(deltaTime);  // 자식이 자기 기준으로 다시 이동
    }
}

void Motor::MoveByParentDelta(const CPoint& delta)
{
    strPos += delta;
    endPos += delta;
    motorPos += delta;

    for (auto child : children)
    {
        child->MoveByParentDelta(delta);  // 하위-하위도 한 번만 위치 이동
    }
}
