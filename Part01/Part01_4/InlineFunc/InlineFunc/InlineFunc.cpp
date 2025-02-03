#include <iostream>

//inline int SQUARE(int x) {
//	return x * x;
//}

template <typename T>
inline T SQUARE(T x) {
	return x * x;
}

int main(void) {
	std::cout << SQUARE(5.5) << std::endl;
	std::cout << SQUARE(12) << std::endl;
	return 0;
}