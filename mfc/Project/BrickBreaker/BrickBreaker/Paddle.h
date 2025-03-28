#pragma once
#include "GameObject.h"

class Paddle : public GameObject {
private:
	bool movingLeft = false;
	bool movingRight = false;
	bool movingUp = false;
	bool movingDown = false;
public:
	int dx; // 패들의 이동 속도
	int dy;

	Paddle(int x, int y, int width, int height, int dx, int dy)
		: GameObject(x, y, width, height), dx(dx), dy(dy) {
	}

	void Draw(CDC* pDC) override;
	bool Update(CRect boundary, CWnd* pWnd) override;
	void StartMovingLeft() { movingLeft = true; }
	void StartMovingRight() { movingRight = true; }
	void StartMovingUp() { movingUp = true; }
	void StartMovingDown() { movingDown = true; }
	void StopMoving() { movingLeft = false; movingRight = false; movingUp = false; movingDown = false; }
};