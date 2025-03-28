#pragma once

class GameObject {
public:
	int x, y;
	int width, height;

	GameObject() : x(0), y(0), width(0), height(0) {
		// 기본 생성자
	}

	GameObject(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height) {
	}

	virtual void Draw(CDC* pDC) = 0;
	virtual bool Update(CRect boundary, CWnd* pWnd) = 0;
};
