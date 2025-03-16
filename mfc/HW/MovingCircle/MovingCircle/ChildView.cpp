// ChildView.cpp

#include "pch.h"
#include "framework.h"
#include "MovingCircle.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
    m_x = 150;
    m_y = 150;
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
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_BN_CLICKED(101, &CChildView::OnSpeedUp)
    ON_BN_CLICKED(102, &CChildView::OnSpeedDown)
    ON_BN_CLICKED(103, &CChildView::OnChangeDirection)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

    return TRUE;
}

void CChildView::OnPaint()
{
    CPaintDC dc(this);

    // 사각형 그리기
    dc.Rectangle(m_boundary);

    // 원 그리기
    dc.Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        // 충돌 감지 및 방향 반전
        if (m_x + m_radius >= m_boundary.right || m_x - m_radius <= m_boundary.left) {
            m_dx = -m_dx;
        }
        if (m_y + m_radius >= m_boundary.bottom || m_y - m_radius <= m_boundary.top) {
            m_dy = -m_dy;
        }

        // 위치 업데이트
        m_x += m_dx;
        m_y += m_dy;

        InvalidateRect(NULL, FALSE);
    }

    CWnd::OnTimer(nIDEvent);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    m_boundary.left = 10;
    m_boundary.top = 10;
    m_boundary.right = cx - 120;
    m_boundary.bottom = cy - 10;

    int controlX = cx - 110;

    if (m_speedUp.m_hWnd)
    {
        m_speedUp.MoveWindow(controlX, 10, 100, 40);
        m_speedDown.MoveWindow(controlX, 60, 100, 40);
        m_changeDirection.MoveWindow(controlX, 110, 100, 40);
        m_checkX.MoveWindow(controlX, 160, 100, 20);
        m_checkY.MoveWindow(controlX, 190, 100, 20);
    }
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetTimer(1, 20, NULL);

    // 버튼 컨트롤 생성
    m_speedUp.Create(_T("속도 ↑"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(0, 0, 100, 40), this, 101);
    m_speedDown.Create(_T("속도 ↓"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(0, 0, 100, 40), this, 102);
    m_changeDirection.Create(_T("방향 변경"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(0, 0, 100, 40), this, 103);

    // 체크 박스 컨트롤 생성
    m_checkX.Create(_T("X 방향"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        CRect(0, 0, 100, 20), this, 104);
    m_checkY.Create(_T("Y 방향"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        CRect(0, 0, 100, 20), this, 105);

    return 0;
}

void CChildView::OnDestroy()
{
    KillTimer(1);
    CWnd::OnDestroy();
}

void CChildView::OnSpeedUp()
{
    if (abs(m_dx) < 50) m_dx *= 2;
    if (abs(m_dy) < 50) m_dy *= 2;
}

void CChildView::OnSpeedDown()
{
    if (abs(m_dx) > 1) m_dx /= 2;
    if (abs(m_dy) > 1) m_dy /= 2;
}

void CChildView::OnChangeDirection()
{
    if (m_checkX.GetCheck() == BST_CHECKED) {
        m_dx = -m_dx;
    }
    if (m_checkY.GetCheck() == BST_CHECKED) {
        m_dy = -m_dy;
    }
}