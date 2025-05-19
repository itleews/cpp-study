#pragma once

#include "MotorAxis.h"
#include <vector>
#include <fstream>
#include <sstream>

class MotorManager {
public:
	std::vector<Motor*> rootMotors;

	int nextId = 1;
	
	Motor* AddMotor(
		Motor* parentMotor,
		bool isXDirection,
		CPoint strPos, CPoint endPos, CPoint motorPos,
		CSize motorSize, int motorSpeed);

	Motor* ParseMotor(const std::string& line);
	void SaveMotorData();
	void SaveMotorRecursive(std::ofstream& file, Motor* motor);
	void LoadMotorData();
};