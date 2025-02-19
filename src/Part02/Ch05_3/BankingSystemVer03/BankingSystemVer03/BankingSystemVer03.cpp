/*
* Banking System Ver 0.3
* 작성자: 이우식
* 내  용: Account 클래스 복사 생성자 정의
*/

#include <iostream>
#include <cstring>

using namespace std;
const int NAME_LEN = 20;

void ShowMenu(void); // 메뉴 출력
void MakeAccount(void); // 계좌개설을 위한 함수
void DepositMoney(void); // 입금을 위한 함수
void WithdrawMoney(void); // 출금을 위한 함수
void ShowAllAccInfo(void); // 잔액조회를 위한 함수

enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT };

class Account
{
private:
	int accID;
	int balance;
	char* cusName;

public:
	Account(int ID, int money, char* name)
		: accID(ID), balance(money)
	{
		cusName = new char[strlen(name) + 1];
		strcpy_s(cusName, strlen(name) + 1, name);
	}

	Account(const Account& ref)
		: accID(ref.accID), balance(ref.balance)
	{
		cusName = new char[strlen(ref.cusName) + 1];
		strcpy_s(cusName, strlen(ref.cusName) + 1, ref.cusName);
	}

	int GetAccID() { return accID; }

	void Deposit(int money)
	{
		balance += money;
	}

	int Withdraw(int money)
	{
		if (balance < money)
			return 0;
		balance -= money;
		return money;
	}

	void ShowAccInfo()
	{
		cout << accID << "\t " << cusName << "\t " << balance << endl;
	}

	~Account()
	{
		delete[] cusName;
	}
};

Account* accArr[100]; // Account 저장을 위한 배열
int accNum = 0; // 저장된 Account 수

int main(void)
{
	int choice;
	while (1)
	{
		ShowMenu();
		cout << "선택: ";
		cin >> choice;
		cout << endl;
		switch (choice)
		{
		case MAKE:
			MakeAccount();
			break;
		case DEPOSIT:
			DepositMoney();
			break;
		case WITHDRAW:
			WithdrawMoney();
			break;
		case INQUIRE:
			ShowAllAccInfo();
			break;
		case EXIT:
			for (int i = 0; i < accNum; i++)
				delete accArr[i];
			return 0;
		default:
			cout << "Illegal selection.." << endl;
			return 0;
		}
	}
}

void ShowMenu(void)
{
	cout << "-----Menu-----" << endl;
	cout << "1. 계좌개설" << endl;
	cout << "2. 입 금" << endl;
	cout << "3. 출 금" << endl;
	cout << "4. 계좌정보 전체 출력" << endl;
	cout << "5. 프로그램 종료" << endl;
}

void MakeAccount(void)
{
	int id;
	char name[NAME_LEN];
	int balance;
	cout << "[계좌개설]" << endl;
	cout << "계좌ID: "; cin >> id;
	cout << "이름: "; cin >> name;
	cout << "입금액: "; cin >> balance;
	cout << endl;
	accArr[accNum++] = new Account(id, balance, name);
}

void DepositMoney(void)
{
	int money;
	int id;
	cout << "[입	 금]" << endl;
	cout << "계좌ID: "; cin >> id;
	cout << "입금액: "; cin >> money;
	cout << endl;
	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			accArr[i]->Deposit(money);
			cout << "입금완료" << endl << endl;
			return;
		}
	}
	cout << "유효하지 않은 ID입니다." << endl << endl;
}

void WithdrawMoney(void)
{
	int money;
	int id;
	cout << "[출	 금]" << endl;
	cout << "계좌ID: "; cin >> id;
	cout << "출금액: "; cin >> money;
	cout << endl;
	for (int i = 0; i < accNum; i++)
	{
		if (accArr[i]->GetAccID() == id)
		{
			if (accArr[i]->Withdraw(money) == 0)
			{
				cout << "잔액부족" << endl << endl;
				return;
			}
			cout << "출금완료" << endl << endl;
			return;
		}
	}
	cout << "유효하지 않은 ID입니다." << endl << endl;
}

void ShowAllAccInfo(void)
{
	cout << "[계좌정보 전체 출력]" << endl;
	cout << "계좌ID\t 이 름\t 잔 액" << endl;
	cout << "------------------------" << endl;
	if (accNum == 0)
	{
		cout << "등록된 계좌가 없습니다." << endl << endl;
		return;
	}
	else {
		for (int i = 0; i < accNum; i++)
		{
			accArr[i]->ShowAccInfo();
			cout << endl;
		}
	}
}