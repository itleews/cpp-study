#pragma once

#include "MotorAxis.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class MotorManager {
public:
	int nextId = 0;
	std::vector<Motor*> rootMotors;

	std::map<int, Motor*> motorMap;
	std::vector<std::pair<Motor*, int>> motorsWithParents;  // Motor*, parentId Ω÷ ¿˙¿Â
	
	Motor* AddMotor(
		Motor* parentMotor,
		bool isXDirection,
		CPoint strPos, CPoint endPos, CPoint motorPos,
		CSize motorSize, int motorSpeed);

	int GetMaxId();
	void SaveMotorData();
	void SaveMotorRecursive(std::ofstream& file, Motor* motor, CPoint parentOffset = (0, 0));
	void LoadMotorData();
	Motor* ParseMotor(const std::string& line, int& outParentId);
};