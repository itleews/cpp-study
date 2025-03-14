#include <iostream>
using namespace std;

#if 0
/* ��� ������ */
int main() {
	int a;
	int b;
	const int* pa = &a;

	*pa = 3; // �ùٸ��� ���� ����
	pa = &b; // �ùٸ� ����

	return 0;
}
#endif

#if 0
/* ��� ������ */
int main() {
	int a;
	int b;
	int* const pa = &a;

	*pa = 3; // �ùٸ� ����
	pa = &b; // �ùٸ��� ���� ����

	return 0;
}
#endif

#if 0
/* �������� ���� */
int main() {
	int a;
	int* pa;
	pa = &a;

	cout << "pa�� ��: " << pa << endl;
	cout << "(pa + 1)�� ��: " << pa + 1 << endl;

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

	cout << "pa�� ��: " << pa << endl;
	cout << "(pa + 1)�� ��: " << pa + 1 << endl; // int�� �������̹Ƿ� 4����Ʈ ����
	cout << "pb�� ��: " << static_cast<void*>(pb) << endl;
	cout << "(pb + 1)�� ��: " << static_cast<void*>(pb + 1) << endl; // char�� �������̹Ƿ� 1����Ʈ ����
	cout << "pc�� ��: " << pc << endl;
	cout << "(pc + 1)�� ��: " << pc + 1 << endl; // double�� �������̹Ƿ� 8����Ʈ ����

	return 0;
}
#endif

#if 0
/* �������� ���� */
int main() {
	int a;
	int* pa = &a;

	cout << "pa�� ��: " << pa << endl;
	cout << "(pa - 1)�� ��: " << pa - 1 << endl;

	return 0;
}
#endif

#if 0
/* �������� ���� */
int main() {
	int a;
	int* pa = &a;
	int* pb;

	*pa = 3;
	pb = pa;

	cout << "pa�� ����Ű�� �ִ� �� : " << *pa << endl;
	cout << "pb�� ����Ű�� �ִ� �� : " << *pb << endl;

	return 0;
}
#endif

#if 0
/* �迭�� ���� ���� */
int main() {
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	for (int i = 0; i < 10; i++) {
		cout << "arr[" << i << "]�� �ּ� : " << &arr[i] << endl;
	}

	return 0;
}
#endif

#if 0
/* �迭�� ���Ҹ� ����Ű�� ������ */
int main() {
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int* parr;
	parr = &arr[0];

	for (int i = 0; i < 10; i++) {
		cout << "arr[" << i << "]�� �ּҰ� : " << &arr[i];
		cout << " (parr + " << i << ")�� �� : " << parr + i;

		if (&arr[i] == (parr + i)) {
			/* ���� (parr + i)�� ���������� arr[i]�� ����Ų�ٸ� */
			cout << " ---> ��ġ \n";
		}
		else {
			cout << " ---> ����ġ \n";
		}
	}

	return 0;
}
#endif

#if 0
/* �����ͷ� �迭�� ���ҿ� �����ϱ� */
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

	cout << "arr�� ��ü : " << arr << endl;
	cout << "arr[0]�� �ּ� : " << &arr[0] << endl;

	return 0;
}
#endif

#if 0
/* [] ������ */
int main() {
	int arr[5] = { 1, 2, 3, 4, 5 };

	cout << "a[3] : " << arr[3] << endl;
	cout << "*(a + 3) : " << *(arr + 3) << endl;

	return 0;
}
#endif