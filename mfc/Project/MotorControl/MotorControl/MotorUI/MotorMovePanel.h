#pragma once
#include "../Motor/MotorManager.h"

class MotorMovePanel {
public:
	void MoveMotorRecursive(Motor* motor, int dx, int dy);
};