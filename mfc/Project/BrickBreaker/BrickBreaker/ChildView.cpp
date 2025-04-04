
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "BrickBreaker.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	// 엔터키 눌렀을 때 버튼이 포커스되어 있으면 클릭 처리
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus() == &m_startButton)
		{
			m_startButton.SendMessage(BM_CLICK);
			return TRUE; // 처리했으니 리턴
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(1, &CChildView::OnStartButton)
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
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rect;
	GetClientRect(&rect);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(&rect, RGB(255, 255, 255)); // 배경

	memDC.Rectangle(m_boundary);
	m_gameManager.DrawGame(&memDC);

	ULONGLONG elapsedMillis = GetTickCount64() - m_startTick;
	int minutes = (elapsedMillis / (1000 * 60)) % 60;
	int seconds = (elapsedMillis / 1000) % 60;
	int milliseconds = elapsedMillis % 1000;

	LOGFONT logFont = { 0 };
	logFont.lfHeight = 48;
	_tcscpy_s(logFont.lfFaceName, _T("Arial"));
	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont* pOldFont = memDC.SelectObject(&font);

	CString strTime;
	strTime.Format(_T("%02d:%02d.%03d"), minutes, seconds, milliseconds);
	memDC.TextOutW(rect.right - 190, 10, strTime);

	int score = 2000 - m_gameManager.m_brickCount * 10;
	CString strScore;
	strScore.Format(_T("점수: %d"), score);
	memDC.TextOutW(rect.right - 190, 60, strScore);

	memDC.SelectObject(pOldFont);
	
	// 게임 시작 전: 텍스트만 출력
	if (!m_isRunning) {
		LOGFONT logFont = { 0 };
		logFont.lfHeight = 60;
		_tcscpy_s(logFont.lfFaceName, _T("Arial"));
		CFont font;
		font.CreateFontIndirect(&logFont);
		CFont* pOldFont = memDC.SelectObject(&font);

		CString title = _T("블록깨기 게임");
		CString instruction = _T("시작하려면 \"게임 시작\" 버튼을 누르세요!");

		CSize textSize = memDC.GetTextExtent(title);
		CSize instructionSize = memDC.GetTextExtent(instruction);

		int x = (rect.Width() - textSize.cx) / 2;
		int y = (rect.Height() - textSize.cy) / 2;
		memDC.TextOutW(x, y, title);
		memDC.TextOutW(x, y + textSize.cy + 10, instruction);

		memDC.SelectObject(pOldFont);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
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
				if (m_gameManager.balls.size() > 1) {
					m_gameManager.DestroyBall(&ball);
				}
				else {
					m_isRunning = false;
					m_gameManager.DestroyBall(&ball);
					m_gameManager.EndGame(this);
					MessageBoxA(GetSafeHwnd(), "공이 떨어졌습니다...", "패배", MB_OK | MB_ICONERROR);
					m_startButton.EnableWindow(TRUE);
					m_startButton.SetFocus();

					//if (AfxMessageBox(_T("게임을 다시 시작하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
					//	Invalidate(FALSE);
					//	m_gameManager.ResetGame(m_boundary, this);
					//	SetTimer(1, 16, nullptr);
					//}
					//else {
					//	PostQuitMessage(0); // 프로그램 종료
					//}
					return; // 더 이상 업데이트하지 않음
				}
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

	// 버튼 생성
	m_startButton.Create(_T("게임 시작"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(10, 10, 100, 30), this, 1);
	m_startButton.SetFocus();

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

		// 버튼 위치 조정
		m_startButton.MoveWindow(cx - 190, 200, 180, 50);
	}
}

void CChildView::OnStartButton() {
	// 게임 시작 버튼 클릭 시
	if (!m_isRunning) {
		m_startTick = GetTickCount64();
		m_gameManager.ResetGame(m_boundary, this);
		m_isRunning = true;
		SetTimer(1, 16, nullptr);
		SetFocus();
		m_startButton.EnableWindow(FALSE);
	}
	else {
		m_isRunning = false;
		KillTimer(1);
		m_startButton.EnableWindow(TRUE);
	}
	Invalidate(TRUE);
}

