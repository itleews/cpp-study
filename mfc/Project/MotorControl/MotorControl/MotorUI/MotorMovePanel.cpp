#include "pch.h"
#include "MotorMovePanel.h"

void MotorMovePanel::MoveMotorRecursive(Motor* motor, int dx, int dy)
{
	if (!motor) return;

	if (motor->parentMotor) {
		CPoint parentTopLeft = motor->parentMotor->motorPos - motor->parentMotor->motorSize;
		CPoint parentBottomRight = motor->parentMotor->motorPos + motor->parentMotor->motorSize;

		CPoint newStrPos = motor->strPos + CPoint(dx, dy);
		CPoint newEndPos = motor->endPos + CPoint(dx, dy);

		if (newStrPos.x < parentTopLeft.x || newStrPos.y < parentTopLeft.y ||
			newEndPos.x > parentBottomRight.x || newEndPos.y > parentBottomRight.y)
		{
			return; // �θ� ������ ����Ƿ� �̵����� ����
		}
	}

	motor->strPos.x += dx;
	motor->strPos.y += dy;
	motor->endPos.x += dx;
	motor->endPos.y += dy;
	motor->motorPos.x += dx;
	motor->motorPos.y += dy;

	for (Motor* child : motor->children)
	{
		MoveMotorRecursive(child, dx, dy);
	}
}


