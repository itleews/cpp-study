#include <iostream>
using namespace std;

int main(void) {
	int n;
	cout << "보고 싶은 단을 입력하세요: ";
	cin >> n;

	for (int i = 1; i <= 9; i++) {
		cout << n << " * " << i << " = " << n * i << endl;
	}

	return 0;
}