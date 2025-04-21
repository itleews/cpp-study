#pragma once

#include "MotorAxis.h"
#include <vector>

class MotorManager {
public:
	std::vector<MotorAxis*> axisList;
	CRect baseRect;       // 현재 기준 사각형
	CRect prevBaseRect;   // 이전 기준 사각형 (비율 보정용)

	int nextId = 1;
	
	MotorAxis* AddAxis(
		bool isXDirection,
		double strXRatio, double strYRatio,
		double endXRatio, double endYRatio,
		double motorXRatio, double motorYRatio,
		CSize motorSize);
	//void MoveAxis(int id, CPoint dest);
	MotorAxis* FindAxis(int id);
	void UpdateAllMotorRatios();
};