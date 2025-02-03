#include <iostream>
using namespace std;

int main(void) {
	int val[5];

	for (int i = 0; i < 5; i++) {
		cout << i + 1 << "번째 정수 입력: ";
		cin >> val[i];
	}

	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += val[i];
	}

	cout << "합계: " << sum << endl;
	return 0;
}