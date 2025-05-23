
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MotorControl.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
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
	CRect clientRect;
	GetClientRect(&clientRect);

	// 1. 메모리 DC 생성 (더블 버퍼링)
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	// 전체 배경 회색
	memDC.FillSolidRect(clientRect, RGB(240, 240, 240));

	// 영역 나누기
	memDC.FillSolidRect(m_drawArea, RGB(255, 255, 255));
	memDC.FrameRect(m_drawArea, &CBrush(RGB(0, 0, 0)));

	// 왼쪽 클리핑 설정
	memDC.IntersectClipRect(m_drawArea);

	DrawGrid(&memDC); // 그리드 그리기

	// 도형 그리기
	for (auto axis : m_motorUI.m_motorManager.rootMotors)
	{
		DrawMotor(axis, &memDC);
		// 하위 모터 그리기 (재귀적으로)
		DrawSubMotor(axis, &memDC);
	}

	if (m_motorUI.m_isAddSubmotorMode)
	{
		DrawAddSubmotorMode(&memDC);
	}

	if (m_motorUI.m_isAddMotorMode || m_motorUI.m_isAddSubmotorMode)
	{
		// 모터 추가 모드일 때는 미리보기 모터 그리기
		DrawPreviewMotor(&memDC);
	}

	if (m_isPaused)
	{
		CPen redPen(PS_SOLID, 5, RGB(255, 0, 0));
		CPen* pOldPen = memDC.SelectObject(&redPen);
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.Rectangle(m_drawArea);
		memDC.SelectObject(pOldPen);

		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(RGB(255, 0, 0));
		memDC.TextOut(m_drawArea.left + 50, m_drawArea.top + 30, _T("일시정지"));
	}

	// 최종 결과를 실제 DC에 복사
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);

	// 메모리 DC 정리
	memDC.SelectObject(pOldBitmap);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_motorUI.Create(NULL, _T("Motor UI"), WS_CHILD | WS_VISIBLE,
			CRect(300, 300, 500, 500), this, 1001))
	{
		AfxMessageBox(_T("MotorUI 생성 실패"));
	}
	else
	{
		m_motorUI.SetParentView(this);
	}

	SetTimer(1, 16, NULL);

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// 도형 그리는 영역: 왼쪽 80%
	m_drawArea.SetRect(10, 10, (int)(cx * 0.8) - 20, cy - 10);

	if (::IsWindow(m_motorUI.GetSafeHwnd()))
	{
		// 오른쪽 20%에 motorUI 배치
		int uiX = (int)(cx * 0.8);
		m_motorUI.MoveWindow(uiX, 10, cx - uiX - 10, cy - 20);
	}
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// 다시 그리기
		InvalidateRect(&m_drawArea, FALSE);

		if (m_motorUI.m_isAddSubmotorMode || m_motorUI.m_isSaveMotorMode)
		{
			m_isPaused = true; // 일시정지
			return;
		}

		m_isPaused = false; // 일시정지 해제
		double deltaTime = 0.016; // 약 60fps = 16ms 주기

		// 모든 모터 위치를 갱신
		for (auto axis : m_motorUI.m_motorManager.rootMotors)
		{
			axis->MoveMotor(deltaTime);
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CChildView::OnDestroy()
{
	KillTimer(1);  // 타이머 ID 1 제거
	CWnd::OnDestroy();
}

void CChildView::DrawGrid(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(200, 200, 200)); 
	CPen* pOldPen = pDC->SelectObject(&pen); // 기존 펜을 교체

	// 그리드 선을 논리 좌표계 기준으로 그림
	for (int x = m_logicalBounds.left; x <= m_logicalBounds.right; x += 10)
	{
		// 논리 좌표 → 화면 좌표
		CPoint screenStart = LogicalToScreen(CPoint(x, m_logicalBounds.top));
		CPoint screenEnd = LogicalToScreen(CPoint(x, m_logicalBounds.bottom));

		pDC->MoveTo(screenStart);
		pDC->LineTo(screenEnd);
	}

	for (int y = m_logicalBounds.top; y <= m_logicalBounds.bottom; y += 10)
	{
		// 논리 좌표 → 화면 좌표
		CPoint screenStart = LogicalToScreen(CPoint(m_logicalBounds.left, y));
		CPoint screenEnd = LogicalToScreen(CPoint(m_logicalBounds.right, y));

		pDC->MoveTo(screenStart);
		pDC->LineTo(screenEnd);
	}

	// 펜을 원래대로 돌려놓음
	pDC->SelectObject(pOldPen);
}

void CChildView::DrawMotor(Motor* motor, CDC* pDC) {
	// 화면 좌표로 변환
	CPoint screenStart = LogicalToScreen(motor->strPos);
	CPoint screenEnd = LogicalToScreen(motor->endPos);
	CPoint screenMotorStart = LogicalToScreen(motor->motorPos - motor->motorSize);
	CPoint screenMotorEnd = LogicalToScreen(motor->motorPos + motor->motorSize);

	pDC->MoveTo(screenStart.x, screenStart.y);
	pDC->LineTo(screenEnd.x, screenEnd.y);

	CRect motorRect(screenMotorStart.x, screenMotorStart.y, screenMotorEnd.x, screenMotorEnd.y);
	pDC->FillSolidRect(motorRect, RGB(255, 255, 255));
	pDC->FrameRect(motorRect, &CBrush(RGB(0, 0, 0)));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);

	CString str;
	str.Format(_T("ID:%d"), motor->m_id);
	pDC->TextOutW(screenStart.x, screenStart.y - 30, str);
}

// 하위 모터를 재귀적으로 그리기 위한 함수
void CChildView::DrawSubMotor(Motor* parentMotor, CDC* pDC)
{
	// 하위 모터가 있는 경우 재귀적으로 그리기
	for (Motor* child : parentMotor->children)
	{
		DrawMotor(child, pDC); // 자식 모터 그리기

		// 자식의 하위 모터도 그리기 (재귀 호출)
		DrawSubMotor(child, pDC);
	}
}

void CChildView::DrawPreviewMotor(CDC* pDC) {
	// 원래 로직 좌표 기준으로 사각형 생성
	Motor preMotor = m_motorUI.m_previewMotor;
	CPoint screenStart = LogicalToScreen(preMotor.strPos);
	CPoint screenEnd = LogicalToScreen(preMotor.endPos);
	CPoint screenMotorStart = LogicalToScreen(preMotor.motorPos - preMotor.motorSize);
	CPoint screenMotorEnd = LogicalToScreen(preMotor.motorPos + preMotor.motorSize);

	// 점선 펜 생성
	CPen pen(PS_DASH, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	// 브러시는 NULL로 설정
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	// 변환된 좌표로 사각형 그리기
	CRect previewRect;
	if (m_motorUI.m_previewMotor.isX) {
		previewRect.SetRect(screenStart.x, screenMotorStart.y - 10, screenEnd.x, screenMotorEnd.y + 10);
	}
	else {
		previewRect.SetRect(screenMotorStart.x - 10, screenStart.y, screenMotorEnd.x + 10, screenEnd.y);
	}
	pDC->Rectangle(previewRect);

	// 펜과 브러시 복원
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	// 모터 도형은 기존 논리 좌표 기준으로 그려짐
	DrawMotor(&m_motorUI.m_previewMotor, pDC);
}

void CChildView::DrawAddSubmotorMode(CDC* pDC)
{
	CRect rect = m_drawArea; // 그리드 영역 사용
	CRect selectedRect = m_motorUI.m_selectedMotorRect;

	if (!selectedRect.IsRectEmpty())
	{
		CPoint motorSize = selectedRect.BottomRight() - selectedRect.TopLeft();
		CPoint LogicalStart = LogicalToScreen(selectedRect.TopLeft());
		CPoint LogicalEnd = LogicalToScreen(selectedRect.BottomRight());
		selectedRect = CRect(LogicalStart, LogicalEnd);
		pDC->ExcludeClipRect(selectedRect);

		// 좌상단 좌표 텍스트 출력
		CString topLeftText;
		topLeftText.Format(_T("(0, 0)"));
		pDC->TextOutW(selectedRect.left, selectedRect.top - 20, topLeftText); // 위쪽에 약간 띄워서 출력

		// 우하단 좌표 텍스트 출력
		CString bottomRightText;
		bottomRightText.Format(_T("(%d, %d)"), motorSize.x, motorSize.y);
		pDC->TextOutW(selectedRect.right, selectedRect.bottom + 20, bottomRightText); // 아래쪽에 약간 띄워서 출력
	}

	// 그림자 배경 준비
	COLORREF shadowColor = RGB(0, 0, 0);
	BYTE alpha = 100;

	BLENDFUNCTION blendFunc = { 0 };
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = alpha;
	blendFunc.AlphaFormat = 0;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), shadowColor);

	// Alpha blending
	pDC->AlphaBlend(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, rect.Width(), rect.Height(), blendFunc);

	memDC.SelectObject(pOldBmp);

	// 클리핑 복원
	pDC->SelectClipRgn(NULL);

	CFont bigFont;
	bigFont.CreateFontW(
		40, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("맑은 고딕")
	);
	CFont* pOldFont = pDC->SelectObject(&bigFont);
	pDC->SetTextColor(RGB(255, 0, 0));  // ⬅️ 여기에 있어야 함
	pDC->SetBkMode(TRANSPARENT);       // 배경 투명 설정도 같이 해주면 좋아!

	CString msg = _T("모터 목록에서 하위 모터를 추가할 상위 모터를 선택하세요");
	CSize textSize = pDC->GetTextExtent(msg);
	pDC->TextOutW((rect.left + rect.right) / 2, rect.top + 50, msg);

	pDC->SelectObject(pOldFont);  // 폰트 복원도 잊지 마!
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const double zoomStep = 0.1;
	if (zDelta > 0)
		m_zoomFactor *= (1.0 + zoomStep);  // 확대
	else
		m_zoomFactor *= (1.0 - zoomStep);  // 축소

	InvalidateRect(m_drawArea, FALSE); // 해당 영역만 갱신
	return TRUE;
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 위치 (point.x, point.y)를 비율 좌표로 변환
	CPoint logicalPoint = ScreenToLogical(point);

	// 마우스 위치를 상태 표시줄에 표시할 텍스트로 변환
	CString str;
	str.Format(_T("Position: X = %d, Y = %d"), logicalPoint.x, logicalPoint.y);

	// CMainFrame 포인터 얻기 (정확하게 CMainFrame으로 캐스팅)
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		// 상태 표시줄의 4번째 패널에 마우스 위치 텍스트 설정
		pMainFrame->m_wndStatusBar.SetPaneText(1, str);
	}

	if (m_isPanning) {
		CPoint delta = point - m_lastMousePos;
		m_panOffset += delta;
		m_lastMousePos = point;

		InvalidateRect(m_drawArea, FALSE); // 해당 영역만 갱신
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_drawArea.PtInRect(point)) {
		m_isPanning = true;
		m_lastMousePos = point;
		SetCapture();  // 마우스 캡처
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isPanning) {
		m_isPanning = false;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}