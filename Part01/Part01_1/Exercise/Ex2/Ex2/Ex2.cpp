#include <iostream>
using namespace std;

int main(void) {
	char name[100];
	char phone[100];

	cout << "이름을 입력하세요: ";
	cin >> name;

	cout << "전화번호를 입력하세요: ";
	cin >> phone;

	cout << "이름: " << name << endl;
	cout << "전화번호: " << phone << endl;

	return 0;
}