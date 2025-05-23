
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "MotorManager.h"
#include "MotorUI.h"

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	MotorUI m_motorUI;
	CRect m_drawArea; // 그리기 영역
	double m_zoomFactor = 2.5;
	CPoint m_mousePos;
	CPoint m_lastMousePos;
	CPoint m_panOffset{ 0, 0 };
	bool m_isPanning = false;
	CRect m_logicalBounds = CRect(0, 0, 1000, 1000);
	int m_dashOffset = 0;
	bool m_isPaused = false;

// 작업입니다.
public:
	CPoint LogicalToScreen(CPoint logical)
	{
		// X와 Y의 위치를 논리 좌표계에서 화면 좌표계로 변환
		int screenX = (int)((logical.x - m_logicalBounds.left) * m_zoomFactor + m_panOffset.x);
		int screenY = (int)((logical.y - m_logicalBounds.top) * m_zoomFactor + m_panOffset.y);
		return CPoint(screenX, screenY);
	}

	// 화면 좌표 → 논리 좌표 변환
	CPoint ScreenToLogical(CPoint screen)
	{
		int logicalX = (int)((screen.x - m_panOffset.x) / m_zoomFactor + m_logicalBounds.left);
		int logicalY = (int)((screen.y - m_panOffset.y) / m_zoomFactor + m_logicalBounds.top);
		return CPoint(logicalX, logicalY);
	}

// 재정의입니다.
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();
	void DrawGrid(CDC* pDC);
	void DrawMotor(Motor* motor, CDC* pDC);
	void DrawSubMotor(Motor* parentMotor, CDC* pDC);
	void DrawAddSubmotorMode(CDC* pDC);
	void DrawPreviewMotor(CDC* pDC);
	void OnDestroy();
	
	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

