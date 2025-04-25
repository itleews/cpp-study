#include "pch.h"
#include "MotorAxis.h"

MotorAxis::MotorAxis(int id, bool isXDirection,
	CPoint str, CPoint end, CPoint motor, CSize size)
	: m_id(id), isX(isXDirection),
	strPos(str), endPos(end), motorPos(motor),
	motorSize(size) {
}

//void MotorAxis::MoveTo(CPoint dest) {
//    position = dest;
//}