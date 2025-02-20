/*
* 파일이름: HighCreditAccount.h
* 작성자: 이우식
* 업데이트 정보: [2025, 02, 20] 파일버전 0.92
*/

#ifndef __HIGHCREDIT_ACCOUNT_H__
#define __HIGHCREDIT_ACCOUNT_H__

#include "NormalAccount.h"
#include "String.h"
#include "AccountException.h"

class HighCreditAccount : public NormalAccount
{
private:
	int specialRate;
public:
	HighCreditAccount(int ID, int money, String name, int rate, int special)
		: NormalAccount(ID, money, name, rate), specialRate(special)
	{
	}
	virtual void Deposit(int money)
	{
		if (money < 0)
			throw MinusException(money);

		NormalAccount::Deposit(money); // 원금과 이자추가
		Account::Deposit(money * (specialRate / 100.0)); // 특별이자추가
	}
};
#endif