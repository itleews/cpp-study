#include <iostream>
using namespace std;

#if 0
int main() {
	int arr[3] = { 1, 2, 3 };
	int* parr;

	parr = arr; // parr = &arr[0];도 동일하다!

	cout << "arr[1] : " << arr[1] << endl;
	cout << "parr[1] : " << parr[1] << endl;
	return 0;
}
#endif

#if 0
/* 포인터 이용하기 */
int main() {
	int arr[10] = { 100, 98, 97, 95, 89, 76, 92, 96, 100, 99 };

	int* parr = arr;
	int sum = 0;

	while (parr - arr <= 9) {
		sum += *parr;
		parr++;
	}

	cout << "내 시험 점수 평균 : " << sum / 10 << endl;
	return 0;
}
#endif

#if 0
/* 포인터의 포인터 */
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
/* 2차원 배열 */
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

	cout << "전체 크기 : " << sizeof(arr) << endl;
	cout << "총 열의 개수 : " << sizeof(arr[0]) / sizeof(arr[0][0]) << endl;
	cout << "총 행의 개수 : " << sizeof(arr) / sizeof(arr[0]) << endl;
	
	return 0;
}
#endif

#if 0
/* 배열의 포인터 */
int main() {
	int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };
	int(*parr)[3]; // <배열의 형>(* <포인터 이름>) [<2차원 배열의 열 개수>]

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
	parr = crr; // 오류! Why? 열의 개수가 다르기 때문에

	return 0;
}
#endif

#if 1
/* 포인터 배열 */
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