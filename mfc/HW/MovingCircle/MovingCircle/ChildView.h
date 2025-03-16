
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once


// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	int m_x, m_y;      // 원의 현재 위치
	int m_radius;      // 원의 반지름
	int m_dx, m_dy;
	CRect m_boundary;
	CButton m_speedUp;
	CButton m_speedDown;
	CButton m_checkX;
	CButton m_checkY;
	CButton m_changeDirection;

// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnChangeDirection();
	afx_msg void OnSpeedUp();
	afx_msg void OnSpeedDown();
};

