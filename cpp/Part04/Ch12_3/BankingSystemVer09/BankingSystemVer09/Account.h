/*
* 파일이름: Account.h
* 작성자: 이우식
* 업데이트 정보: [2025, 02, 19] 파일버전 0.9
*/

#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include "String.h"

class Account
{
private:
	int accID;
	int balance;
	String cusName;
public:
	Account(int ID, int money, String name);

	int GetAccID() const;
	virtual void Deposit(int money);
	int Withdraw(int money);
	void ShowAccInfo() const;
};
#endif