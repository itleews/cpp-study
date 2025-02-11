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
	cout << "러시아 면적: " << CountryArea::RUSSIA << "㎢" << endl;
	cout << "캐나다 면적: " << CountryArea::CANADA << "㎢" << endl;
	cout << "중국 면적: " << CountryArea::CHINA << "㎢" << endl;
	cout << "한국 면적: " << CountryArea::SOUTH_KOREA << "㎢" << endl;
	return 0;
}