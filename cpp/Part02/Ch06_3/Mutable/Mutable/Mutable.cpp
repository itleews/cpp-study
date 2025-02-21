#include <iostream>
using namespace std;

class SoSimple {
private:
	int num1;
	mutable int num2;
public:
	SoSimple(int n1, int n2) : num1(n1), num2(n2) {}
	void ShowSimpleData() const {
		cout << num1 << ", " << num2 << endl;
	}
	void CopyToNum2() const {
		num2 = num1;
	}
};

int main(void) {
	SoSimple sim(1, 2);
	sim.ShowSimpleData();
	sim.CopyToNum2();
	sim.ShowSimpleData();
	return 0;
}