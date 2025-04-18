#pragma once

enum { IDD = IDD_MOTOR_LIST_FORM };

class CMotorListView : public CFormView
{
protected:
	CMotorListView();           // 동적 생성 전용
	DECLARE_DYNCREATE(CMotorListView)

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOTOR_LIST_FORM };
#endif

	// 멤버 변수
protected:
	CListCtrl m_motorList; // 모터 리스트 컨트롤

	// 함수
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원
	virtual void OnInitialUpdate(); // 처음 한 번만 호출됨

	// 구현
protected:
	virtual ~CMotorListView();
	DECLARE_MESSAGE_MAP()
};
