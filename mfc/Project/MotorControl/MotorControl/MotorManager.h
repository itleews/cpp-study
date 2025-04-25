#pragma once

#include "MotorAxis.h"
#include <vector>

class MotorManager {
public:
	std::vector<MotorAxis*> axisList;

	int nextId = 1;
	
	MotorAxis* AddAxis(
		bool isXDirection,
		CPoint strPos, CPoint endPos, CPoint motorPos,
		CSize motorSize);
	//void MoveAxis(int id, CPoint dest);
	MotorAxis* FindAxis(int id);
	/*void UpdateAllMotorRatios(CRect prevBaseRect, CRect newBaseRect);*/
};