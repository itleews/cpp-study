#include <iostream>
using namespace std;

#if 0
int main() {
	int arr[3] = { 1, 2, 3 };
	int* parr;

	parr = arr; // parr = &arr[0];�� �����ϴ�!

	cout << "arr[1] : " << arr[1] << endl;
	cout << "parr[1] : " << parr[1] << endl;
	return 0;
}
#endif

#if 0
/* ������ �̿��ϱ� */
int main() {
	int arr[10] = { 100, 98, 97, 95, 89, 76, 92, 96, 100, 99 };

	int* parr = arr;
	int sum = 0;

	while (parr - arr <= 9) {
		sum += *parr;
		parr++;
	}

	cout << "�� ���� ���� ��� : " << sum / 10 << endl;
	return 0;
}
#endif

#if 0
/* �������� ������ */
int main() {
	int a;
	int* pa;
	int** ppa;

	pa = &a;
	ppa = &pa;

	a = 3;

	cout << "a : " << a << " // *pa : " << *pa << " // **ppa : " << **ppa << endl;
	cout << "&a : " << &a << " // pa : " << pa << " // *ppa : " << *ppa << endl;
	cout << "&pa : " << &pa << " // ppa : " << ppa << endl;
	
	return 0;
}
#endif

#if 0
int main() {
	int arr[3] = { 1, 2, 3 };
	int (*parr)[3] = &arr;

	cout << "arr[1] : " << arr[1] << endl;
	cout << "(*parr)[1] : " << (*parr)[1] << endl;
	cout << "arr : " << arr << endl;
	cout << "parr : " << parr << endl;
	return 0;
}
#endif

#if 0
/* 2���� �迭 */
int main() {
	int arr[2][3];

	cout << "arr[0] : " << arr[0] << endl;
	cout << "arr[0][0] : " << &arr[0][0] << endl;

	cout << "arr[1] : " << arr[1] << endl;
	cout << "arr[1][0] : " << &arr[1][0] << endl;

	return 0;
}
#endif

#if 0
int main() {
	int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };

	cout << "��ü ũ�� : " << sizeof(arr) << endl;
	cout << "�� ���� ���� : " << sizeof(arr[0]) / sizeof(arr[0][0]) << endl;
	cout << "�� ���� ���� : " << sizeof(arr) / sizeof(arr[0]) << endl;
	
	return 0;
}
#endif

#if 0
/* �迭�� ������ */
int main() {
	int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };
	int(*parr)[3]; // <�迭�� ��>(* <������ �̸�>) [<2���� �迭�� �� ����>]

	parr = arr;

	cout << "parr[1][2] : " << parr[1][2] << ", arr[1][2] : " << arr[1][2] << endl;

	return 0;
}
#endif

#if 0
int main() {
	int arr[2][3];
	int brr[10][3];
	int crr[2][5];

	int(*parr)[3];

	parr = arr; // OK
	parr = brr; // OK
	parr = crr; // ����! Why? ���� ������ �ٸ��� ������

	return 0;
}
#endif

#if 1
/* ������ �迭 */
int main() {
	int* arr[3];
	int a = 1, b = 2, c = 3;
	arr[0] = &a;
	arr[1] = &b;
	arr[2] = &c;

	cout << "a : " << a << ", *arr[0] : " << *arr[0] << endl;
	cout << "b : " << b << ", *arr[1] : " << *arr[1] << endl;
	cout << "c : " << c << ", *arr[2] : " << *arr[2] << endl;

	cout << "&a : " << &a << ", arr[0] : " << arr[0] << endl;
	return 0;
}
#endif