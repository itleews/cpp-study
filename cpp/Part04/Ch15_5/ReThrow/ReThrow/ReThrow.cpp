#include <iostream>
using namespace std;

void Divide(int num1, int num2) {
	try {
		if (num2 == 0) {
			throw 0;
		}
		cout << "³ª´°¼ÀÀÇ ¸ò: " << num1 / num2 << endl;
		cout << "³ª´°¼ÀÀÇ ³ª¸ÓÁö: " << num1 % num2 << endl;
	}
	catch (int expn) {
		cout << "first catch" << endl;
		throw; // ¿¹¿Ü¸¦ ´Ù½Ã ´øÁø´Ù!
	}
}

int main(void) {
	try {
		Divide(9, 2);
		Divide(4, 0);
	}
	catch (int expn) {
		cout << "second catch" << endl;
	}
	return 0;
}