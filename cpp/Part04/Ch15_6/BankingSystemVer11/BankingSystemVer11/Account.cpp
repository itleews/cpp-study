/*
* 파일이름: Account.cpp
* 작성자: 이우식
* 업데이트 정보: [2025, 02, 20] 파일버전 0.92
*/

#include "BankingCommonDecl.h"
#include "Account.h"
#include "AccountException.h"

Account::Account(int ID, int money, String name)
	: accID(ID), balance(money)
{
	cusName = name;
}

int Account::GetAccID() const { return accID; }

void Account::Deposit(int money)
{
	if (money < 0)
		throw MinusException(money);

	balance += money;
}

int Account::Withdraw(int money)
{
	if (money < 0)
		throw MinusException(money);

	if (balance < money)
		throw InsuffException(balance, money);

	balance -= money;
	return money;
}

void Account::ShowAccInfo() const
{
	cout << "계좌ID: " << accID << endl;
	cout << "이 름: " << cusName << endl;
	cout << "잔 액: " << balance << endl;
}