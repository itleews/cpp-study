#include <iostream>

int SimpleFunc(int a = 10) {
	return a + 1;
}

// int SimpleFunc(void) {
// 	return 10;
// }
// �⺻ �μ�(void)�� ����ϴ� �Լ��� �Ű������� ���� �Լ��� �浹��.

int main() {
	std::cout << SimpleFunc() << std::endl;
	std::cout << SimpleFunc(20) << std::endl;
	return 0;
}