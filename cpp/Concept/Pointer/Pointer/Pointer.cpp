#include <iostream>
using namespace std;

#if 0
int main() {
	int* p;
	int a;
	
	p = &a;

	cout << "������ p�� ��� �ִ� �� : " << p << endl;
	cout << "int ���� a�� ����� �ּ� : " << &a << endl;
	return 0;
}
#endif

#if 0
int main() {
	int* p;
	int a;

	p = &a;
	a = 2;

	cout << "a�� �� : " << a << endl;
	cout << "*p�� �� : " << *p << endl;

	return 0;
}
#endif

#if 0
int main() {
	int* p;
	int a;

	p = &a;
	*p = 3;

	cout << "a�� �� : " << a << endl;
	cout << "*p�� �� : " << *p << endl;
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