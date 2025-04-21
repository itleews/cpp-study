#include "pch.h"
#include "MotorAxis.h"

MotorAxis::MotorAxis(int id, bool isXDirection,
	double sX, double sY, double eX, double eY, double mX, double mY, CSize size)
	: m_id(id), isX(isXDirection),
	strXRatio(sX), strYRatio(sY),
	endXRatio(eX), endYRatio(eY),
	motorXRatio(mX), motorYRatio(mY),
	motorSize(size) {
}

//void MotorAxis::MoveTo(CPoint dest) {
//    position = dest;
//}