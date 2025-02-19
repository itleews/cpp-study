#include <iostream>
using namespace std;

class Point {
private:
	int xpos, ypos;
public:
	Point(int x, int y) : xpos(x), ypos(y) {}
	void ShowPointInfo() const {
		cout << "[" << xpos << ", " << ypos << "]" << endl;
	}
};

class Circle {
private:
	Point center;
	int radius;
public:
	Circle(int x, int y, int r) : center(x, y), radius(r) {}
	void ShowCircleInfo() const {
		cout << "radius: " << radius << endl;
		center.ShowPointInfo();
	}
};

class Ring {
private:
	Circle innerCircle;
	Circle outterCircle;
public:
	Ring(int inX, int inY, int inR, int outX, int outY, int outR) : innerCircle(inX, inY, inR), outterCircle(outX, outY, outR) {}
	void ShowRingInfo() const {
		cout << "Inner Circle Info..." << endl;
		innerCircle.ShowCircleInfo();
		cout << "Outter Circle Info..." << endl;
		outterCircle.ShowCircleInfo();
	}
};

int main(void) {
	Ring ring(1, 1, 4, 2, 2, 9);
	ring.ShowRingInfo();
	return 0;
}