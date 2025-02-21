/*
* �����̸�: AccountException.h
* �ۼ���: �̿��
* ������Ʈ ����: [2025, 02, 20] ���Ϲ��� 0.92
*/

#ifndef __ACCOUNT_EXCEPTION_H__
#define __ACCOUNT_EXCEPTION_H__
#include <iostream>
using namespace std;

class MinusException
{
private:
	int exval; // ������ ������ �Ǵ� �ݾ�

public:
	MinusException(int val) : exval(val) {}
	void ShowExceptionInfo(void) const
	{
		cout << "��(��)�ݾ� " << exval << "�� ��ȿ���� �ʽ��ϴ�!" << endl;
	}
};

class InsuffException
{
private:
	int balance; // ���� �ܾ�
	int reqval; // ��� �䱸��
public:
	InsuffException(int val, int req) : balance(val), reqval(req) {}
	void ShowExceptionInfo() const
	{
		cout << "�ܾ׿��� " << reqval - balance << "��(��) �����մϴ�!" << endl;
	}
};

#endif