#include <iostream>
#include <cstring>
using namespace std;

class MyFriendInfo {
private:
	char* name;
	int age;
public:
	MyFriendInfo(const char* fname, int fage) : age(fage) {
		name = new char[strlen(fname) + 1];
		strcpy_s(name, strlen(fname) + 1, fname);
	}
	~MyFriendInfo() {
		delete[] name;
	}
	void ShowMyFriendInfo() {
		cout << "이름: " << name << endl;
		cout << "나이: " << age << endl;
	}
};

class MyFriendDetailInfo : public MyFriendInfo {
private:
	char* addr;
	char* phone;
public:
	MyFriendDetailInfo(const char* fname, int fage, const char* faddr, const char* fphone) : MyFriendInfo(fname, fage) {
		addr = new char[strlen(faddr) + 1];
		strcpy_s(addr, strlen(faddr) + 1, faddr);
		phone = new char[strlen(fphone) + 1];
		strcpy_s(phone, strlen(fphone) + 1, fphone);
	}
	~MyFriendDetailInfo() {
		delete[] addr;
		delete[] phone;
	}
	void ShowMyFriendDetailInfo() {
		ShowMyFriendInfo();
		cout << "주소: " << addr << endl;
		cout << "전화: " << phone << endl << endl;
	}
};

int main() {
	MyFriendDetailInfo fren1("김진성", 22, "충남 아산", "010-1234-00XX");
	MyFriendDetailInfo fren2("이주성", 19, "경기 인천", "010-3333-00XX");
	fren1.ShowMyFriendDetailInfo();
	fren2.ShowMyFriendDetailInfo();
	return 0;
}