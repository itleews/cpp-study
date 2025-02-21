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
	SoSimple* simPtr = new SoComplex;
	SoComplex* comPtr = dynamic_cast<SoComplex*>(simPtr);
	comPtr->ShowSimpleInfo();
	return 0;
}