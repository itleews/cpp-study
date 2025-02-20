#include <iostream>
using namespace std;

class AAA {
public:
	void ShowYou() {
		cout << "AAA exception" << endl;
	}
};

class BBB : public AAA {
public:
	void ShowYou() {
		cout << "BBB exception" << endl;
	}
};

class CCC : public BBB {
public:
	void ShowYou() {
		cout << "CCC exception" << endl;
	}
};

void ExceptionGenerator(int expn) {
	if (expn == 1)
		throw AAA();
	else if (expn == 2)
		throw BBB();
	else if (expn == 3)
		throw CCC();
}

int main2(void) {
	try {
		ExceptionGenerator(1);
		ExceptionGenerator(2);
		ExceptionGenerator(3);
	}
	catch (AAA& expn) {
		cout << "catch(AAA& expn)" << endl;
		expn.ShowYou();
	}
	catch (BBB& expn) {
		cout << "catch(BBB& expn)" << endl;
		expn.ShowYou();
	}
	catch (CCC& expn) {
		cout << "catch(CCC& expn)" << endl;
		expn.ShowYou();
	}
	return 0;
}