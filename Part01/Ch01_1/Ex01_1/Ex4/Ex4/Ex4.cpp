#include <iostream>
using namespace std;

int main(void) {
	while(1) {
		int sales, salary = 0;
		cout << "�Ǹ� �ݾ��� ���� ������ �Է�(-1 to end): ";
		cin >> sales;

		if (sales == -1) break;
		salary = 50 + sales * 0.12;
		cout << "�̹� �� �޿�: " << salary << "����" << endl;
	}

	cout << "���α׷��� �����մϴ�." << endl;
	return 0;
}