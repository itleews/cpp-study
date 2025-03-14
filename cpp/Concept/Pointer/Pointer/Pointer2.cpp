#include <iostream>
using namespace std;

#if 0
/* 상수 포인터 */
int main() {
	int a;
	int b;
	const int* pa = &a;

	*pa = 3; // 올바르지 않은 문장
	pa = &b; // 올바른 문장

	return 0;
}
#endif

#if 0
/* 상수 포인터 */
int main() {
	int a;
	int b;
	int* const pa = &a;

	*pa = 3; // 올바른 문장
	pa = &b; // 올바르지 않은 문장

	return 0;
}
#endif

#if 0
/* 포인터의 덧셈 */
int main() {
	int a;
	int* pa;
	pa = &a;

	cout << "pa의 값: " << pa << endl;
	cout << "(pa + 1)의 값: " << pa + 1 << endl;

	return 0;
}
#endif

#if 0
int main() {
	int a;
	char b;
	double c;
	int* pa = &a;
	char* pb = &b;
	double* pc = &c;

	cout << "pa의 값: " << pa << endl;
	cout << "(pa + 1)의 값: " << pa + 1 << endl; // int형 포인터이므로 4바이트 증가
	cout << "pb의 값: " << static_cast<void*>(pb) << endl;
	cout << "(pb + 1)의 값: " << static_cast<void*>(pb + 1) << endl; // char형 포인터이므로 1바이트 증가
	cout << "pc의 값: " << pc << endl;
	cout << "(pc + 1)의 값: " << pc + 1 << endl; // double형 포인터이므로 8바이트 증가

	return 0;
}
#endif

#if 0
/* 포인터의 뺄셈 */
int main() {
	int a;
	int* pa = &a;

	cout << "pa의 값: " << pa << endl;
	cout << "(pa - 1)의 값: " << pa - 1 << endl;

	return 0;
}
#endif

#if 0
/* 포인터의 대입 */
int main() {
	int a;
	int* pa = &a;
	int* pb;

	*pa = 3;
	pb = pa;

	cout << "pa가 가리키고 있는 것 : " << *pa << endl;
	cout << "pb가 가리키고 있는 것 : " << *pb << endl;

	return 0;
}
#endif

#if 0
/* 배열의 존재 상태 */
int main() {
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	for (int i = 0; i < 10; i++) {
		cout << "arr[" << i << "]의 주소 : " << &arr[i] << endl;
	}

	return 0;
}
#endif

#if 0
/* 배열의 원소를 가리키는 포인터 */
int main() {
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int* parr;
	parr = &arr[0];

	for (int i = 0; i < 10; i++) {
		cout << "arr[" << i << "]의 주소값 : " << &arr[i];
		cout << " (parr + " << i << ")의 값 : " << parr + i;

		if (&arr[i] == (parr + i)) {
			/* 만일 (parr + i)가 성공적으로 arr[i]를 가리킨다면 */
			cout << " ---> 일치 \n";
		}
		else {
			cout << " ---> 불일치 \n";
		}
	}

	return 0;
}
#endif

#if 0
/* 포인터로 배열의 원소에 접근하기 */
int main() {
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int* parr;

	parr = &arr[0];

	cout << "arr[3] = " << arr[3] << ", *(parr + 3) = " << *(parr + 3) << endl;
	return 0;
}
#endif

#if 0
int main() {
	int arr[3] = { 1, 2, 3 };

	cout << "arr의 정체 : " << arr << endl;
	cout << "arr[0]의 주소 : " << &arr[0] << endl;

	return 0;
}
#endif

#if 0
/* [] 연산자 */
int main() {
	int arr[5] = { 1, 2, 3, 4, 5 };

	cout << "a[3] : " << arr[3] << endl;
	cout << "*(a + 3) : " << *(arr + 3) << endl;

	return 0;
}
#endif