#include <iostream>
using namespace std;

class CountryArea {
public:
	static const int RUSSIA = 1707540;
	static const int CANADA = 998467;
	static const int CHINA = 957290;
	static const int SOUTH_KOREA = 9922;
};

int main(void) {
	cout << "���þ� ����: " << CountryArea::RUSSIA << "��" << endl;
	cout << "ĳ���� ����: " << CountryArea::CANADA << "��" << endl;
	cout << "�߱� ����: " << CountryArea::CHINA << "��" << endl;
	cout << "�ѱ� ����: " << CountryArea::SOUTH_KOREA << "��" << endl;
	return 0;
}