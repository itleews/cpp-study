#include <iostream>
#include <cstring>
using namespace std;

class Girl; // Gir1이라는 이름이 클래스의 이름임을 알림

class Boy {
private:
	int height;
	friend class Girl; // Gir1 클래스에 대해 private 멤버에 접근 권한 부여
public:
	Boy(int len) : height(len) {}
	void ShowYourFriendInfo(Girl& frn);
};

class Girl {
private:
	char phNum[20];
public:
	Girl(const char* num) {
		strcpy_s(phNum, num);
	}
	void ShowYourFriendInfo(Boy& frn);
	friend class Boy;
};

void Boy::ShowYourFriendInfo(Girl& frn) {
	cout << "Her phone number: " << frn.phNum << endl;
}

void Girl::ShowYourFriendInfo(Boy& frn) {
	cout << "His height: " << frn.height << endl;
}

int main(void) {
	Boy boy(170);
	Girl girl("010-1234-5678");
	boy.ShowYourFriendInfo(girl);
	girl.ShowYourFriendInfo(boy);
	return 0;
}