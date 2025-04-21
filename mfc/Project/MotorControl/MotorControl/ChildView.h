
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "MotorManager.h"

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	MotorManager m_motorManager;
	CButton m_addMotorButton;
	CButton m_removeMotorButton;
	CButton m_saveMotorButton;
	CButton m_loadMotorButton;
	CButton m_motorControlStatic;
	CButton m_groupInput, m_groupButtons;
	CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit; // 시작 위치 입력
	CEdit m_width, m_height;
	CStatic m_labelStart, m_labelEnd;
	CStatic m_labelSize, m_labelAxis;
	CButton m_radioXAxis, m_radioYAxis;
    CRect m_baseRect = CRect(0, 0, 600, 600);  // 기준 모터의 영역
	CRect m_drawArea; // 그리기 영역

// 작업입니다.
public:
	BOOL m_hasBaseRect = FALSE; // 기준 모터가 설정되었는지 여부

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddMotor();
	afx_msg void OnRemoveMotor();
	afx_msg void OnSaveMotor();
	afx_msg void OnLoadMotor();
	afx_msg void OnBnClickedRadioXAxis();
	afx_msg void OnBnClickedRadioYAxis();
	afx_msg void OnChangeStartX();
	afx_msg void OnChangeStartY();
	DECLARE_MESSAGE_MAP()

private:
	// 모터 리스트를 위한 CListCtrl
	CListCtrl m_motorListCtrl;
};

