#pragma once

enum { IDD = IDD_MOTOR_LIST_FORM };

class CMotorListView : public CFormView
{
public:
	CMotorListView();           // ���� ���� ����
	DECLARE_DYNCREATE(CMotorListView)

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOTOR_LIST_FORM };
#endif

	// ��� ����
public:
	CListCtrl m_motorList; // ���� ����Ʈ ��Ʈ��

	// �Լ�
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����
	virtual void OnInitialUpdate(); // ó�� �� ���� ȣ���

	// ����
public:
	virtual ~CMotorListView();
	DECLARE_MESSAGE_MAP()
};
