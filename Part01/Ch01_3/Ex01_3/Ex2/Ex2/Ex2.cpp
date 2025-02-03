#include <iostream>

int SimpleFunc(int a = 10) {
	return a + 1;
}

// int SimpleFunc(void) {
// 	return 10;
// }
// 기본 인수(void)를 사용하는 함수와 매개변수가 없는 함수가 충돌함.

int main() {
	std::cout << SimpleFunc() << std::endl;
	std::cout << SimpleFunc(20) << std::endl;
	return 0;
}