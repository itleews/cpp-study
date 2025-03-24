// ChildView.cpp

#include "pch.h"
#include "framework.h"
#include "MovingCircle.h"
#include "ChildView.h"
#include <cstdlib> // rand() 함수를 사용하기 위해 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
	m_circleCount = 1;
    m_circles.resize(m_circleCount);
    srand(time(NULL));
    for (int i = 0; i < m_circleCount; i++) {
        int randomX = std::rand() % 1000;
		int randomY = std::rand() % 1000;
		int randomSpeed = std::rand() % 10 + 1;
        m_circles[i] = CCircle(randomX, randomY, 20, randomSpeed, randomSpeed);
        m_circles[i].m_circleColor = RGB(rand() % 256, rand() % 256, rand() % 256);
    }
}

CChildView::~CChildView()
{
}

CCircle::~CCircle()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_EN_CHANGE(200, &CChildView::OnEditChanged)
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

    // 원 그리기
    for (int i = 0; i < m_circleCount; i++) {
        // 브러시 생성
        CBrush brush(m_circles[i].m_circleColor);
        CBrush* pOldBrush = memDC.SelectObject(&brush);

        memDC.Ellipse(m_circles[i].m_x - m_circles[i].m_radius, m_circles[i].m_y - m_circles[i].m_radius,
            m_circles[i].m_x + m_circles[i].m_radius, m_circles[i].m_y + m_circles[i].m_radius);

        // 원래 브러시로 복원
        memDC.SelectObject(pOldBrush);
    }
    
    // 메모리 DC의 내용을 화면 DC로 복사
    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    // 메모리 DC 정리
    memDC.SelectObject(pOldBitmap);
    bitmap.DeleteObject();
    ReleaseDC(&memDC);
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
		m_editCtrl.MoveWindow(controlX, 10, 70, 30);
        m_spinCtrl.MoveWindow(controlX + 70, 10, 30, 30);
        m_speedUp.MoveWindow(controlX, 50, 100, 40);
        m_speedDown.MoveWindow(controlX, 100, 100, 40);
        // m_speedDisplay.MoveWindow(controlX, 110, 100, 20);
        m_changeDirection.MoveWindow(controlX, 150, 100, 40);
        m_checkX.MoveWindow(controlX, 200, 100, 20);
        m_checkY.MoveWindow(controlX, 230, 100, 20);
    }
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        // 이전 위치 저장
        for (int i = 0; i < m_circleCount; i++) {
            int old_x = m_circles[i].m_x;
            int old_y = m_circles[i].m_y;

            // Move 메서드에 CWnd* (this)를 넘겨서 무효화 처리
            if (this != nullptr) {
                m_circles[i].Move(m_boundary, this);  // this는 CChildView 객체
            }
        }
    }

    CWnd::OnTimer(nIDEvent);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetTimer(1, 20, NULL);

    // 스핀 컨트롤과 에디트 컨트롤 생성
    m_editCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        CRect(0, 0, 100, 30), this, 200);

    m_spinCtrl.Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_SETBUDDYINT,
        CRect(0, 0, 100, 30), this, 201);

    // 스핀 버튼을 에디트 컨트롤과 연동
    m_spinCtrl.SetBuddy(&m_editCtrl);
    m_spinCtrl.SetRange(1, 100); // 범위 설정
    m_spinCtrl.SetPos(m_circleCount); // 기본값

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
    /*
    CString str;
    str.Format(_T("%d"), abs(m_dx));
    m_speedDisplay.Create(str, WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 100, 20), this, 106);
    */

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
    for (int i = 0; i < m_circleCount; i++) {
        m_circles[i].SpeedUp(MaxSpeed);
    }
    // UpdateSpeedDisplay();
}

void CChildView::OnSpeedDown()
{
    int MinSpeed = 1;
    for (int i = 0; i < m_circleCount; i++) {
        m_circles[i].SpeedDown(MinSpeed);
    }
    // UpdateSpeedDisplay();
}

void CChildView::OnChangeDirection()
{
	for (int i = 0; i < m_circleCount; i++) {
		m_circles[i].ChangeDirection(m_checkX.GetCheck(), m_checkY.GetCheck());
	}
}

void CChildView::OnEditChanged()
{
    // 에디트 컨트롤의 값을 가져오기
    CString str;
    m_editCtrl.GetWindowText(str);

    // 값이 비어있는 경우 아무것도 하지 않음
    if (str.IsEmpty()) {
        return;
    }

    int editValue = _ttoi(str);  // 문자열을 정수로 변환

    if (editValue < 1 || editValue > 100) {
        MessageBox(_T("유효한 값(0 ~ 100)을 입력해 주세요."), _T("경고"), MB_OK | MB_ICONERROR);
    }
    else if (editValue != m_circleCount) {
		int oldCount = m_circleCount;
        m_circleCount = editValue;
        m_circles.resize(m_circleCount);  // 벡터 크기 변경

        // 새 원 추가
        for (int i = oldCount; i < m_circleCount; i++) {
            int randomX = std::rand() % 1000;
            int randomY = std::rand() % 1000;
            int randomSpeed = std::rand() % 10 + 1;
            m_circles[i] = CCircle(randomX, randomY, 20, randomSpeed, randomSpeed);
            m_circles[i].m_circleColor = RGB(rand() % 256, rand() % 256, rand() % 256);
        }

        Invalidate(); // 다시 그리기 요청
    }
}




/*
void CChildView::UpdateSpeedDisplay()
{
    CString str;
    str.Format(_T("%d"), abs(m_dx));
    m_speedDisplay.SetWindowText(str);
}
*/