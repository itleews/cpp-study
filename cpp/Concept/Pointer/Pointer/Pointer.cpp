#include <iostream>
using namespace std;

#if 0
int main() {
	int* p;
	int a;
	
	p = &a;

	cout << "포인터 p에 들어 있는 값 : " << p << endl;
	cout << "int 변수 a가 저장된 주소 : " << &a << endl;
	return 0;
}
#endif

#if 0
int main() {
	int* p;
	int a;

	p = &a;
	a = 2;

	cout << "a의 값 : " << a << endl;
	cout << "*p의 값 : " << *p << endl;

	return 0;
}
#endif

#if 0
int main() {
	int* p;
	int a;

	p = &a;
	*p = 3;

	cout << "a의 값 : " << a << endl;
	cout << "*p의 값 : " << *p << endl;
}
#endif

#if 0
int main() {
	int a;
	int b;
	int* p;

	p = &a;
	*p = 2;
	p = &b;
	*p = 4;

	cout << "a : " << a << endl;
	cout << "b : " << b << endl;
}
#endif