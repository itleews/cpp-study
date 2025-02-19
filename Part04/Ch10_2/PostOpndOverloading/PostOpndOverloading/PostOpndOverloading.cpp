#include <iostream>
using namespace std;

class Point {
private:
	int xpos, ypos;
public:
	Point(int x = 0, int y = 0) : xpos(x), ypos(y) {}
	void ShowPosition() const {
		cout << '[' << xpos << ", " << ypos << ']' << endl;
	}
	Point& operator++() { // ��������
		xpos += 1;
		ypos += 1;
		return *this;
	}
	Point& operator++(int) { // ��������
		Point retobj(xpos, ypos); // const Point retoobj(*this);
		xpos += 1;
		ypos += 1;
		return retobj;
	}
	friend Point& operator--(Point& ref);
	friend Point& operator--(Point& ref, int);
};

Point& operator--(Point& ref) { // ��������
	ref.xpos -= 1;
	ref.ypos -= 1;
	return ref;
}

Point& operator--(Point& ref, int) { // ��������
	Point retobj(ref);
	ref.xpos -= 1;
	ref.ypos -= 1;
	return retobj;
}

int main(void) {
	Point pos(3, 5);
	Point cpy;
	cpy = pos--;
	cpy.ShowPosition();
	pos.ShowPosition();

	cpy = pos++;
	cpy.ShowPosition();
	pos.ShowPosition();
	return 0;
}