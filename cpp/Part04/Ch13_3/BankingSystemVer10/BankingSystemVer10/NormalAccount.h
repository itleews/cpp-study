/*
* 파일이름: NormalAccount.h
* 작성자: 이우식
* 업데이트 정보: [2025, 02, 19] 파일버전 0.9
*/

#ifndef __NORMAL_ACCOUNT_H__
#define __NORMAL_ACCOUNT_H__

#include "Account.h"
#include "String.h"

class NormalAccount : public Account
{
private:
	int interRate; // 이자율 %단위
public:
	NormalAccount(int ID, int money, String name, int rate)
		: Account(ID, money, name), interRate(rate)
	{}
	virtual void Deposit(int money)
	{
		Account::Deposit(money); // 원금추가
		Account::Deposit(money * (interRate / 100.0)); // 이자추가
	}
};
#endif