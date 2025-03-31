
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "BrickBreaker.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


// CChildView

CChildView::CChildView()
	: m_isRunning(false)
{
	m_startTick = 0;
	m_boundary.SetRect(0, 0, 100, 100);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
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

    // 게임 상태 업데이트 및 화면 그리기
	m_gameManager.DrawGame(&memDC);

	// 스톱워치 그리기
	if (m_isRunning) {
		ULONGLONG elapsedMillis = GetTickCount64() - m_startTick;
		int hours = (elapsedMillis / (1000 * 60 * 60)) % 24;
		int minutes = (elapsedMillis / (1000 * 60)) % 60;
		int seconds = (elapsedMillis / 1000) % 60;
		int milliseconds = elapsedMillis % 1000;

		// 큰 폰트 설정
		LOGFONT logFont = { 0 };
		logFont.lfHeight = 48; // 폰트 크기 설정
		_tcscpy_s(logFont.lfFaceName, _T("Arial")); // 폰트 이름 설정
		CFont font;
		font.CreateFontIndirect(&logFont);
		CFont* pOldFont = memDC.SelectObject(&font);

		CString strTime;
		strTime.Format(_T("%02d:%02d.%03d"), minutes, seconds, milliseconds);
		memDC.TextOutW(rect.right - 190, 10, strTime);

		// 폰트 정리
		memDC.SelectObject(pOldFont);
	}

	// 메모리 DC의 내용을 화면 DC로 복사
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 메모리 DC 정리
	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT) {
		m_gameManager.paddles[0].StartMovingLeft();
	}
	else if (nChar == VK_RIGHT) {
		m_gameManager.paddles[0].StartMovingRight();
	}
	else if (nChar == VK_UP) {
		m_gameManager.paddles[0].StartMovingUp();
	}
	else if (nChar == VK_DOWN) {
		m_gameManager.paddles[0].StartMovingDown();
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN) {
		m_gameManager.paddles[0].StopMoving();
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CChildView::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 1) {
		if (m_isRunning) {
			Invalidate(FALSE);
		}

		for (auto& ball : m_gameManager.balls) {
			if (!ball.Update(m_boundary, this)) {
				m_gameManager.EndGame(this);
				AfxMessageBox(_T("Game Over!"));

				if (AfxMessageBox(_T("게임을 다시 시작하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
					Invalidate(FALSE);
					m_gameManager.ResetGame(m_boundary, this);
					SetTimer(1, 16, nullptr);
				}
				else {
					PostQuitMessage(0); // 프로그램 종료
				}
				return; // 더 이상 업데이트하지 않음
			}
		}
		for (auto& paddle : m_gameManager.paddles) {
			paddle.Update(m_boundary, this);
		}
		for (auto& brick : m_gameManager.bricks) {
			brick.Update(m_boundary, this);
		}
		m_gameManager.HandleCollisions(this);
	}
	CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (AfxMessageBox(_T("게임을 시작하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
		Invalidate(FALSE);
		SetTimer(1, 16, nullptr);
		m_startTick = GetTickCount64();
		m_isRunning = true;
	}
	else {
		PostQuitMessage(0); // 프로그램 종료
		return -1;
	}

	return 0;
}

void CChildView::OnDestroy()
{
	KillTimer(1); // 타이머 중지
	CWnd::OnDestroy();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
	{
		m_boundary.SetRect(10, 10, cx - 200, cy - 10);
		m_gameManager.ResetGame(m_boundary, this);
	}
}