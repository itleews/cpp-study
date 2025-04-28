#pragma once

#include <vector>

class Motor {
public:
    int m_id;
    bool isX;
	CPoint strPos, endPos, motorPos;
    CSize motorSize;
	std::vector<Motor*> subMotors; // 하위 모터 리스트

    Motor(int id, bool isXDirection, CPoint str, CPoint end, CPoint motor, CSize size);
    /*void MoveTo(CPoint dest);*/
};