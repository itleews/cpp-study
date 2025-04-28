#pragma once

#include "MotorAxis.h"
#include <vector>
#include <fstream>
#include <sstream>

class MotorManager {
public:
	std::vector<Motor*> motorList;

	int nextId = 1;
	
	Motor* AddMotor(
		bool isXDirection,
		CPoint strPos, CPoint endPos, CPoint motorPos,
		CSize motorSize);
	//void MoveAxis(int id, CPoint dest);
	Motor* ParseMotor(const std::string& line);
	void SaveMotorData();
	void LoadMotorData();
};