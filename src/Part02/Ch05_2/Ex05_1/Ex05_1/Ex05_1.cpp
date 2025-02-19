#include <iostream>
#include <cstring>
using namespace std;

namespace COMP_POS {
	enum { CLERK, SENIOR, ASSIST, MANAGER };

	void ShowPositionInfo(int pos) {
		switch (pos) {
		case CLERK:
			cout << "사원" << endl;
			break;
		case SENIOR:
			cout << "주임" << endl;
			break;
		case ASSIST:
			cout << "대리" << endl;
			break;
		case MANAGER:
			cout << "과장" << endl;
		}
	}
}

class NameCard {
private:
	char* name;
	char* company;
	char* phone;
	int position;
public:
	NameCard(const char* _name, const char* _company, const char* _phone, int pos) : position(pos) {
		size_t lenName = strlen(_name) + 1;
		size_t lenCompany = strlen(_company) + 1;
		size_t lenPhone = strlen(_phone) + 1;
		name = new char[lenName];
		company = new char[lenCompany];
		phone = new char[lenPhone];
		strcpy_s(name, lenName, _name);
		strcpy_s(company, lenCompany, _company);
		strcpy_s(phone, lenPhone, _phone);
	}
	NameCard(const NameCard& copy) : position(copy.position) {
		size_t lenName = strlen(copy.name) + 1;
		size_t lenCompany = strlen(copy.company) + 1;
		size_t lenPhone = strlen(copy.phone) + 1;
		name = new char[lenName];
		company = new char[lenCompany];
		phone = new char[lenPhone];
		strcpy_s(name, lenName, copy.name);
		strcpy_s(company, lenCompany, copy.company);
		strcpy_s(phone, lenPhone, copy.phone);
	}
	void ShowNameCardInfo() {
		cout << "이름: " << name << endl;
		cout << "회사: " << company << endl;
		cout << "전화번호: " << phone << endl;
		cout << "직급: "; COMP_POS::ShowPositionInfo(position);
		cout << endl;
	}
	~NameCard() {
		delete[] name;
		delete[] company;
		delete[] phone;
	}
};

int main(void) {
	NameCard manClerk("Lee", "ABCEng", "010-1111-2222", COMP_POS::CLERK);
	NameCard copy1 = manClerk;
	NameCard manSENIOR("Hong", "OrangeEng", "010-3333-4444", COMP_POS::SENIOR);
	NameCard copy2 = manSENIOR;
	copy1.ShowNameCardInfo();
	copy2.ShowNameCardInfo();
	return 0;
}