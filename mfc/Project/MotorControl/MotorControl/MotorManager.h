#pragma once

#include "MotorAxis.h"
#include <vector>

class MotorManager {
public:
	std::vector<MotorAxis*> axisList;
	int nextId = 1;

	MotorAxis* AddAxis(CPoint strPos, CPoint endPos);
	//void MoveAxis(int id, CPoint dest);
	MotorAxis* FindAxis(int id);
};