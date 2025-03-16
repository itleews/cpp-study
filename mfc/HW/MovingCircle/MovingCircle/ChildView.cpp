// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MovingCircle.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_x = 100;
    m_y = 100;
    m_radius = 50;

    m_dx = 10;
    m_dy = 10;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
    ON_WM_CREATE()
    ON_WM_DESTROY()
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
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    // 클라이언트 영역에 맞춰 사각형 그리기
    dc.Rectangle(rect.left + 10, rect.top + 10, rect.right - 500, rect.bottom - 10);

    // 원 그리기
    dc.Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        CRect rect;
        GetClientRect(&rect);

        int width = rect.right - 500 - 10;
        int height = rect.bottom - 10 - 10;

        if (m_x + m_radius >= width || m_x - m_radius <= 10) {
            m_dx = -m_dx;
        }
        if (m_y + m_radius >= height || m_y - m_radius <= 10) {
            m_dy = -m_dy;
        }

        m_x += m_dx;
        m_y += m_dy;

        InvalidateRect(NULL, FALSE);
    }

    CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetTimer(1, 20, NULL);

    // 버튼 컨트롤 생성
    m_speedUp.Create(_T("속도 증가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1000, 100, 100, 100), this, 101);

    return 0;
}

void CChildView::OnDestroy()
{
    KillTimer(1);
    CWnd::OnDestroy();
}
