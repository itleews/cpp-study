#pragma once

#include "MotorAxis.h"
#include <vector>

class MotorManager {
public:
	std::vector<MotorAxis*> axisList;
	CRect baseRect;       // ���� ���� �簢��
	CRect prevBaseRect;   // ���� ���� �簢�� (���� ������)

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