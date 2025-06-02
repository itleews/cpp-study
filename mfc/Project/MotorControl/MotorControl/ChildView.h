#pragma once
#include "Motor/MotorManager.h"
#include "MotorUI/MotorUI.h"
#include "Motor/MotorTransform.h"

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	MotorUI m_motorUI;
	MotorTransform m_motorTransform;
	CRect m_drawArea; // 그리기 영역
	
	CPoint m_mousePos;
	CPoint m_lastMousePos;
	bool m_isPanning = false;
	int m_dashOffset = 0;
	bool m_isPaused = false;

// 작업입니다.
public:

// 재정의입니다.
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();
	void DrawGrid(CDC* pDC);
	void DrawMotor(Motor* motor, CDC* pDC);
	void DrawSubMotor(Motor* parentMotor, CDC* pDC);
	void DrawRotatingMotor(Motor* motor, CDC* pDC);
	void DrawAddSubmotorMode(CDC* pDC);
	void DrawPreviewMotor(CDC* pDC);
	void DrawPreviewRotatingMotor(CDC* pDC);
	void DrawRotatingMotorShape(const Motor& motor, CDC* pDC);
	void DrawRotatedRectLogical(CDC* pDC, CPoint centerLogical, CSize sizeLogical, CPoint rotationCenter, double rotationAngleDeg);

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

