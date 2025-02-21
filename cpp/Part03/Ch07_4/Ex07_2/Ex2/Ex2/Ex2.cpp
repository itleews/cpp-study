#include <iostream>
#include <cstring>
using namespace std;

class Book {
private:
	char* title; // å�� ����
	char* isbn; // ����ǥ�ص�����ȣ
	int price; // å�� ����
public:
	Book(const char* title, const char* isbn, int value)
		: price(value) {
		this->title = new char[strlen(title) + 1];
		this->isbn = new char[strlen(isbn) + 1];
		strcpy_s(this->title, strlen(title) + 1, title);
		strcpy_s(this->isbn, strlen(isbn) + 1, isbn);
	}
	void ShowBookInfo() {
		cout << "����: " << title << endl;
		cout << "ISBN: " << isbn << endl;
		cout << "����: " << price << endl;
	}
	~Book() {
		delete[] title;
		delete[] isbn;
	}
};

class EBook : public Book {
private:
	char* DRMKey; // ���Ȱ��� Ű
public:
	EBook(const char* title, const char* isbn, int value, const char* key)
		: Book(title, isbn, value) {
		DRMKey = new char[strlen(key) + 1];
		strcpy_s(DRMKey, strlen(key) + 1, key);
	}
	void ShowEBookInfo() {
		ShowBookInfo();
		cout << "����Ű: " << DRMKey << endl;
	}
	~EBook() {
		delete[] DRMKey;
	}
};

int main(void) {
	Book book("���� C++", "555-12345-890-0", 20000);
	book.ShowBookInfo();
	cout << endl;
	EBook ebook("���� C++ ebook", "555-12345-890-1", 10000, "fdx9w0i8kiw");
	ebook.ShowEBookInfo();
	return 0;
}