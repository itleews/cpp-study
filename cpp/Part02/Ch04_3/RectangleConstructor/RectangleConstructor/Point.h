#ifndef __POINT_H__
#define __POINT_H__

class Point {
private:
	int x;
	int y;
public:
	Point(const int& xpos, const int& ypos);
	int GetX() const;
	int GetY() const;
	bool SetX(int xpos);
	bool SetY(int ypos);
};

#endif