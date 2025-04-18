#include "pch.h"
#include "MotorAxis.h"

MotorAxis::MotorAxis(int _id, CPoint _pos) : id(_id), position(_pos) {}

void MotorAxis::MoveTo(CPoint dest) {
    position = dest;
}