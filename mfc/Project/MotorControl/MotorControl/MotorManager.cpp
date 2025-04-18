#include "pch.h"
#include "MotorManager.h"

MotorAxis* MotorManager::AddAxis(CPoint strPos, CPoint endPos) {
	MotorAxis* newAxis = new MotorAxis(nextId++, strPos, endPos);
	axisList.push_back(newAxis);
	return newAxis;
}

//void MotorManager::MoveAxis(int id, CPoint dest) {
//	MotorAxis* axis = FindAxis(id);
//	if (axis) {
//		axis->MoveTo(dest);
//	}
//}

MotorAxis* MotorManager::FindAxis(int id) {
	for (MotorAxis* axis : axisList) {
		if (axis->m_id == id) {
			return axis;
		}
	}
	return nullptr;
}