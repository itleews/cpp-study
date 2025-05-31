
// ChildView.h: CChildView 클래스의 인터페이스
//


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
		Matrix3x3 transform = m_motorTransform.makeTransform(
			-m_logicalBounds.left, // x
			-m_logicalBounds.top,  // y
			0.0,				   // 회전 각도(theta)
			m_zoomFactor,		   // x 스케일(sx)
			m_zoomFactor		   // y 스케일(sy)
		);

		Matrix3x3 pan = m_motorTransform.translation(m_panOffset.x, m_panOffset.y);

		// 전체 변환: 팬 * 줌스케일 * 원점이동
		Matrix3x3 viewTransform = pan * transform;

		// 좌표 변환
		POINT screen = viewTransform.transformPoint(logical.x, logical.y);
		return CPoint(screen.x, screen.y);
	}

	CPoint ScreenToLogical(CPoint screen)
	{
		Matrix3x3 invPan = m_motorTransform.translation(-m_panOffset.x, -m_panOffset.y);

		double invZoom = 1.0 / m_zoomFactor;
		Matrix3x3 invScale = m_motorTransform.scaling(invZoom, invZoom);

		Matrix3x3 invOrigin = m_motorTransform.translation(m_logicalBounds.left, m_logicalBounds.top);

		Matrix3x3 inverseTransform = invOrigin * invScale * invPan;

		POINT logical = inverseTransform.transformPoint(screen.x, screen.y);
		return CPoint(logical.x, logical.y);
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
	void DrawRotatingMotor(Motor* motor, CDC* pDC);
	void DrawAddSubmotorMode(CDC* pDC);
	void DrawPreviewMotor(CDC* pDC);
	void DrawPreviewRotatingMotor(CDC* pDC);
	void DrawRotatingMotorShape(const Motor& motor, CDC* pDC);
	
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

