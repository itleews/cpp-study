#include "pch.h"
#include "MotorManager.h"

MotorAxis* MotorManager::AddAxis(CPoint pos) {
	MotorAxis* newAxis = new MotorAxis(nextId++, pos);
	axisList.push_back(newAxis);
	return newAxis;
}

void MotorManager::MoveAxis(int id, CPoint dest) {
	MotorAxis* axis = FindAxis(id);
	if (axis) {
		axis->MoveTo(dest);
	}
}

MotorAxis* MotorManager::FindAxis(int id) {
	for (MotorAxis* axis : axisList) {
		if (axis->id == id) {
			return axis;
		}
	}
	return nullptr;
}