﻿
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "ColorSelect.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_color = RGB(255, 0, 0);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CChildView::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CChildView::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CChildView::OnUpdateColorBlue)
	ON_WM_CONTEXTMENU()
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
	
	CFont font;
	font.CreatePointFont(300, _T("궁서"));
	dc.SelectObject(&font);
	dc.SetTextColor(m_color);

	CRect rect;
	GetClientRect(&rect);
	CString str = _T("메뉴 테스트");
	dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


void CChildView::OnColorRed()
{
	m_color = RGB(255, 0, 0);
	Invalidate();
}

void CChildView::OnColorGreen()
{
	m_color = RGB(0, 255, 0);
	Invalidate();
}

void CChildView::OnColorBlue()
{
	m_color = RGB(0, 0, 255);
	Invalidate();
}

void CChildView::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_color == RGB(255, 0, 0));
}

void CChildView::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_color == RGB(0, 255, 0));
}

void CChildView::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_color == RGB(0, 0, 255));
}

void CChildView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);
	CMenu* pMenu = menu.GetSubMenu(4);
	pMenu->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, AfxGetMainWnd());
}
