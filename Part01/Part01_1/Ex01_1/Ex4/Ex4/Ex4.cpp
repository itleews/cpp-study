#include <iostream>
using namespace std;

int main(void) {
	while(1) {
		int sales, salary = 0;
		cout << "판매 금액을 만원 단위로 입력(-1 to end): ";
		cin >> sales;

		if (sales == -1) break;
		salary = 50 + sales * 0.12;
		cout << "이번 달 급여: " << salary << "만원" << endl;
	}

	cout << "프로그램을 종료합니다." << endl;
	return 0;
}