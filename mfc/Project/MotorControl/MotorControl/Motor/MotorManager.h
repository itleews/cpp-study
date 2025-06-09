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
	
	Motor* AddMotor(Motor* parentMotor, AxisType axis, CPoint strPos, CPoint endPos, CPoint motorPos, CSize motorSize, int motorSpeed, bool isRotating = false, double rotAngle = 0.0, double rotSpeed = 0.0);
	Motor* AddRotatingMotor(Motor* parentMotor, CPoint motorPos, CSize motorSize, double rotationSpeed);

	void SaveMotorData();
	void LoadMotorData();
	Motor* GetMotorById(int id);
	void RemoveMotors(const std::vector<int>& ids);

private:
	int GetMaxId();
	void DeleteMotorRecursive(Motor* motor);
	void SaveMotorRecursive(std::ofstream& file, Motor* motor, CPoint parentOffset = (0, 0));
	Motor* ParseMotor(const std::string& line, int& outParentId);
	Motor* FindMotorRecursive(Motor* motor, int id);
};