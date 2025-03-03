#include <iostream>
#include <cstring>
using namespace std;

class Person {
private:
	int age; // 나이
	char name[50]; // 이름
public:
	Person(int myage, const char* myname) : age(myage) {
		strcpy_s(name, myname);
	}
	void WhatYourName() const {
		cout << "My name is " << name << endl;
	}
	void HowOldAreYou() const {
		cout << "I'm " << age << " years old" << endl;
	}
};

class UnivStudent : public Person {
private:
	char major[50]; // 전공
public:
	UnivStudent(const char* myname, int myage, const char* mymajor)
		: Person(myage, myname) {
		strcpy_s(major, mymajor);
	}
	void WhoAreYou() const {
		WhatYourName();
		HowOldAreYou();
		cout << "My major is " << major << endl << endl;
	}
};

int main(void) {
	UnivStudent ustd1("Lee", 22, "Computer eng.");
	ustd1.WhoAreYou();
	
	UnivStudent ustd2("Yoon", 21, "Electronic eng.");
	ustd2.WhoAreYou();
	return 0;
}