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
    m_radius = 20;

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
    CPaintDC dc(this); // 그리기 DC

    // 메모리 DC 생성
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    // 클라이언트 영역 크기 가져오기
    CRect rect;
    GetClientRect(&rect);

    // 메모리 DC에 호환 비트맵 생성
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // 배경 지우기
    memDC.FillSolidRect(&rect, RGB(255, 255, 255));

    // 사각형 그리기
    memDC.Rectangle(m_boundary);

    // 검은색 브러시 생성
    CBrush brushBlack(RGB(0, 0, 0));
    CBrush* pOldBrush = memDC.SelectObject(&brushBlack);

    // 원 그리기
    memDC.Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // 원래 브러시로 복원
    memDC.SelectObject(pOldBrush);

    // 메모리 DC의 내용을 화면 DC로 복사
    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    // 메모리 DC 정리
    memDC.SelectObject(pOldBitmap);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        // 이전 위치 저장
        int old_x = m_x;
        int old_y = m_y;

        // 충돌 감지 및 방향 반전
        if (m_x + m_radius + m_dx >= m_boundary.right) {
            m_dx = -m_dx;
            m_x = m_boundary.right - m_radius;
        }
        else if (m_x - m_radius + m_dx <= m_boundary.left) {
            m_dx = -m_dx;
            m_x = m_boundary.left + m_radius;
        }

        if (m_y + m_radius + m_dy >= m_boundary.bottom) {
            m_dy = -m_dy;
            m_y = m_boundary.bottom - m_radius;
        }
        else if (m_y - m_radius + m_dy <= m_boundary.top) {
            m_dy = -m_dy;
            m_y = m_boundary.top + m_radius;
        }

        // 위치 업데이트
        m_x += m_dx;
        m_y += m_dy;

        // 이전 위치와 새로운 위치를 포함하는 영역 무효화
        CRect oldRect(old_x - m_radius, old_y - m_radius, old_x + m_radius, old_y + m_radius);
        CRect newRect(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);
        oldRect.UnionRect(&oldRect, &newRect);
        InvalidateRect(&oldRect, FALSE);
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
        m_speedDisplay.MoveWindow(controlX, 110, 100, 20);
        m_changeDirection.MoveWindow(controlX, 140, 100, 40);
        m_checkX.MoveWindow(controlX, 190, 100, 20);
        m_checkY.MoveWindow(controlX, 220, 100, 20);	
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

    // 속도 표시 컨트롤 생성
    CString str;
    str.Format(_T("%d"), abs(m_dx));
    m_speedDisplay.Create(str, WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 100, 20), this, 106);

    return 0;
}

void CChildView::OnDestroy()
{
    KillTimer(1);
    CWnd::OnDestroy();
}

void CChildView::OnSpeedUp()
{
    int MaxSpeed = 1000;
    int CurrentSpeed = abs(m_dx);
    if (CurrentSpeed >= MaxSpeed) {
        MessageBox(_T("최고 속도입니다."), _T("알림"), MB_OK | MB_ICONERROR);
        return;
    }
    if (CurrentSpeed < MaxSpeed) {
        m_dx *= 2;
        m_dy *= 2;
    }
    UpdateSpeedDisplay();
}

void CChildView::OnSpeedDown()
{
    int MinSpeed = 1;
    int CurrentSpeed = abs(m_dx);
	if (CurrentSpeed <= MinSpeed) {
		MessageBox(_T("최저 속도입니다."), _T("알림"), MB_OK | MB_ICONERROR);
		return;
	}
    if (CurrentSpeed > MinSpeed) {
        m_dx /= 2;
        m_dy /= 2;
    }
    UpdateSpeedDisplay();
}

void CChildView::OnChangeDirection()
{
    if (m_checkX.GetCheck() == 0 && m_checkY.GetCheck() == 0) {
        MessageBox(_T("방향을 선택하세요."), _T("알림"), MB_OK | MB_ICONWARNING);
        return;
    }
    if (m_checkX.GetCheck() == BST_CHECKED) {
        m_dx = -m_dx;
    }
    if (m_checkY.GetCheck() == BST_CHECKED) {
        m_dy = -m_dy;
    }
}

void CChildView::UpdateSpeedDisplay()
{
    CString str;
    str.Format(_T("%d"), abs(m_dx));
    m_speedDisplay.SetWindowText(str);
}