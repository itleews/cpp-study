#include <iostream>
using namespace std;

class SoSimple { // Polymorphic Ŭ����! ShowSimpleInfo�� �����Լ��̹Ƿ�
public:
	virtual void ShowSimpleInfo() {
		cout << "SoSimple Base Class" << endl;
	}
};

class SoComplex : public SoSimple {
public:
	void ShowSimpleInfo() { // �̰� ���� �����Լ�!
		cout << "SoComplex Derived Class" << endl;
	}
};

int main(void) {
	SoSimple* simPtr = new SoSimple;
	SoComplex* comPtr = dynamic_cast<SoComplex*>(simPtr);
	if (comPtr == NULL) {
		cout << "�� ��ȯ ����" << endl;
	}
	else {
		comPtr->ShowSimpleInfo();
	}
	return 0;
}