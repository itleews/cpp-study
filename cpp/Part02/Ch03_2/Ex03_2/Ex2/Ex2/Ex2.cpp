#include <iostream>
#include <cstring>
using namespace std;

class Printer {
private:
	char str[30];
public:
	void SetString(const char* s);
	void ShowString();
};

void Printer::SetString(const char* s) {
	strcpy_s(str, s);
}

void Printer::ShowString() {
	cout << str << endl;
}

int main() {
	Printer pnt;
	pnt.SetString("Hello world!");
	pnt.ShowString();
	pnt.SetString("I love C++");
	pnt.ShowString();
	return 0;
}