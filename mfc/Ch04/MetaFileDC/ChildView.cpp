﻿
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MetaFileDC.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	// 확장 메타파일 객체를 생성하고 초기화한다.
	CMetaFileDC dc;
	dc.CreateEnhanced(NULL, NULL, NULL, NULL);
	// 멤버 함수를 호출하여 출력한다.
	dc.Rectangle(0, 0, 10, 10);
	dc.Ellipse(1, 1, 9, 9);
	// 확장 메타파일 핸들을 얻는다.
	m_hmf = dc.CloseEnhanced();
}

CChildView::~CChildView()
{
	// 확장 메타파일을 삭제한다.
	::DeleteEnhMetaFile(m_hmf);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CRect rect(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
	dc.PlayMetaFile(m_hmf, &rect);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CRect rect(point.x - 50, point.y - 20, point.x + 50, point.y + 20);
	dc.PlayMetaFile(m_hmf, &rect);
}
