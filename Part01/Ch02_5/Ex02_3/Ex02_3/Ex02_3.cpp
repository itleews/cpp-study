#include <iostream>
using namespace std;

typedef struct __Point {
	int xpos;
	int ypos;
} Point;

Point& PntAdder(const Point& p1, const Point& p2) {
	Point* ptr = new Point;
	ptr->xpos = p1.xpos + p2.xpos;
	ptr->ypos = p1.ypos + p2.ypos;
	return *ptr;
}

int main(void) {
	Point* pptr1 = new Point;
	pptr1->xpos = 3;
	pptr1->ypos = 30;

	Point* pptr2 = new Point;
	pptr2->xpos = 70;
	pptr2->ypos = 7;

	Point& pref = PntAdder(*pptr1, *pptr2);
	cout << "[" << pref.xpos << ", " << pref.ypos << "]" << endl;

	delete pptr1;
	delete pptr2;
	delete& pref;
	return 0;
}