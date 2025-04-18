#include "pch.h"
#include "MotorAxis.h"

MotorAxis::MotorAxis(int id, CPoint str, CPoint end) : m_id(id), strPos(str), endPos(end) {}

//void MotorAxis::MoveTo(CPoint dest) {
//    position = dest;
//}