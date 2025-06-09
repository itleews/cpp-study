
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
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

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
	for (auto motor : m_motorUI.m_motorManager.rootMotors)
	{
		if (motor->axis == T && motor->isRotating) {
			DrawRotatingMotor(motor, &memDC);
		}
		else {
			DrawMotor(motor, &memDC);
		}
		// 하위 모터 그리기 (재귀적으로)
		DrawSubMotor(motor, &memDC);
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

	if (m_motorUI.m_isAddRotatingMotorMode)
	{
		DrawPreviewRotatingMotor(&memDC);
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

void UpdateMotorRotation(Motor* motor, double deltaTime)
{
	motor->MoveMotor(deltaTime);

	if (motor->isRotating)
	{
		motor->rotationAngle += motor->rotationSpeed * deltaTime;
		if (motor->rotationAngle >= 360.0)
			motor->rotationAngle -= 360.0;
	}

	// 자식 모터들도 재귀 처리
	for (auto childMotor : motor->children)
	{
		UpdateMotorRotation(childMotor, deltaTime);
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
		double deltaTime = 0.032; // 약 60fps = 16ms 주기

		// 모든 모터 위치, 회전각 갱신
		for (auto motor : m_motorUI.m_motorManager.rootMotors)
		{
			UpdateMotorRotation(motor, deltaTime);
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
	
	CRect logicalBounds = m_motorTransform.m_logicalBounds;

	// 그리드 선을 논리 좌표계 기준으로 그림
	for (int x = logicalBounds.left; x <= logicalBounds.right; x += 10)
	{
		// 논리 좌표 → 화면 좌표
		CPoint screenStart = m_motorTransform.LogicalToScreen(CPoint(x, logicalBounds.top));
		CPoint screenEnd = m_motorTransform.LogicalToScreen(CPoint(x, logicalBounds.bottom));

		pDC->MoveTo(screenStart);
		pDC->LineTo(screenEnd);
	}

	for (int y = logicalBounds.top; y <= logicalBounds.bottom; y += 10)
	{
		// 논리 좌표 → 화면 좌표
		CPoint screenStart = m_motorTransform.LogicalToScreen(CPoint(logicalBounds.left, y));
		CPoint screenEnd = m_motorTransform.LogicalToScreen(CPoint(logicalBounds.right, y));

		pDC->MoveTo(screenStart);
		pDC->LineTo(screenEnd);
	}

	// 펜을 원래대로 돌려놓음
	pDC->SelectObject(pOldPen);
}

void CChildView::DrawMotor(Motor* motor, CDC* pDC) {
	// 화면 좌표로 변환
	CPoint screenStart = m_motorTransform.LogicalToScreen(motor->strPos);
	CPoint screenEnd = m_motorTransform.LogicalToScreen(motor->endPos);

	CPoint rotationCenter = m_motorUI.m_motorPreviewPanel.GetRotationCenter(motor);

	if (motor->parentMotor) {
		screenStart = m_motorTransform.LogicalToScreen(motor->strPos, rotationCenter, motor->rotationAngle);
		screenEnd = m_motorTransform.LogicalToScreen(motor->endPos, rotationCenter, motor->rotationAngle);
	}

	pDC->MoveTo(screenStart.x, screenStart.y);
	pDC->LineTo(screenEnd.x, screenEnd.y);

	DrawRotatedRectLogical(pDC, motor->motorPos, motor->motorSize,
		rotationCenter,
		motor->parentMotor ? motor->rotationAngle : 0.0);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);

	CString str;
	str.Format(_T("ID:%d"), motor->m_id);
	pDC->TextOutW(screenStart.x, screenStart.y - 30, str);
}

// Logical 좌표 기준 회전된 사각형 그리기
void CChildView::DrawRotatedRectLogical(CDC* pDC, CPoint centerLogical, CSize sizeLogical, CPoint rotationCenter, double rotationAngleDeg)
{
	CPoint ptsScreen[4];
	m_motorTransform.GetRotatedRectScreenPoints(centerLogical, sizeLogical, rotationCenter, rotationAngleDeg, ptsScreen);

	// 내부 채우기
	CBrush brushWhite(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brushWhite);
	pDC->Polygon(ptsScreen, 4);
	pDC->SelectObject(pOldBrush);

	// 테두리
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);
	pDC->MoveTo(ptsScreen[0]);
	for (int i = 1; i < 4; ++i) pDC->LineTo(ptsScreen[i]);
	pDC->LineTo(ptsScreen[0]);
	pDC->SelectObject(pOldPen);
}

// 하위 모터를 재귀적으로 그리기 위한 함수
void CChildView::DrawSubMotor(Motor* parentMotor, CDC* pDC)
{
	// 하위 모터가 있는 경우 재귀적으로 그리기
	for (Motor* child : parentMotor->children)
	{
		if (child->axis == T && child->isRotating) {
			DrawRotatingMotor(child, pDC);
		}
		else {
			DrawMotor(child, pDC); // 자식 모터 그리기
		}
		// 자식의 하위 모터도 그리기 (재귀 호출)
		DrawSubMotor(child, pDC);
	}
}

void CChildView::DrawPreviewMotor(CDC* pDC) {
	// 원래 로직 좌표 기준으로 사각형 생성
	if (m_motorUI.m_isAddRotatingMotorMode) {
		DrawPreviewRotatingMotor(pDC);
		return;
	}

	Motor preMotor = m_motorUI.m_previewMotor;
	CPoint screenStart = m_motorTransform.LogicalToScreen(preMotor.strPos);
	CPoint screenEnd = m_motorTransform.LogicalToScreen(preMotor.endPos);
	CPoint screenMotorStart = m_motorTransform.LogicalToScreen(preMotor.motorPos - preMotor.motorSize);
	CPoint screenMotorEnd = m_motorTransform.LogicalToScreen(preMotor.motorPos + preMotor.motorSize);
    CPoint rotationCenter = m_motorUI.m_motorPreviewPanel.GetRotationCenter(&preMotor);

	// 점선 펜 생성
	CPen pen(PS_DASH, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	// 브러시는 NULL로 설정
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	int width = (preMotor.axis == X)
		? (preMotor.endPos.x - preMotor.strPos.x) / 2
		: preMotor.motorSize.cx + 10;

	int height = (preMotor.axis == Y)
		? (preMotor.endPos.y - preMotor.strPos.y) / 2
		: preMotor.motorSize.cy + 10;

	CSize rotatedSize(width, height);

	// 회전 포함 화면 좌표 얻기
	CPoint points[4];
	m_motorTransform.GetRotatedRectScreenPoints(
		preMotor.motorPos,
		rotatedSize,
		rotationCenter,
		preMotor.rotationAngle,
		points
	);

	// 회전된 사각형 그리기
	pDC->Polygon(points, 4);

	// 펜과 브러시 복원
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	// 모터 도형은 기존 논리 좌표 기준으로 그려짐
	DrawMotor(&m_motorUI.m_previewMotor, pDC);
}

void CChildView::DrawAddSubmotorMode(CDC* pDC)
{
	CRect rect = m_drawArea; // 그리드 영역 사용

	if (m_motorUI.m_selectedMotor)
	{
		Motor* selectedMotor = m_motorUI.m_selectedMotor;

		CPoint rotationCenter = m_motorUI.m_motorPreviewPanel.GetRotationCenter(selectedMotor);

		// 회전 포함 화면 좌표 얻기
		CPoint pts[4];
		m_motorTransform.GetRotatedRectScreenPoints(
			selectedMotor->motorPos,
			selectedMotor->motorSize,
			rotationCenter,
			(selectedMotor->parentMotor && selectedMotor->axis != T) ? selectedMotor->rotationAngle : 0.0,
			pts
		);

		// 클리핑 영역 제외 (회전된 모터 부분 제외)
		CRgn motorRgn;
		motorRgn.CreatePolygonRgn(pts, 4, WINDING);
		pDC->SelectClipRgn(&motorRgn, RGN_DIFF);

		// 텍스트 출력
		CString topLeftText = _T("(0, 0)");
		CString bottomRightText;
		int width = selectedMotor->motorSize.cx * 2;
		int height = selectedMotor->motorSize.cy * 2;
		bottomRightText.Format(_T("(%d, %d)"), width, height);

		pDC->TextOutW(pts[0].x, pts[0].y - 20, topLeftText);     // 좌상단 위
		pDC->TextOutW(pts[2].x, pts[2].y + 20, bottomRightText); // 우하단 아래
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
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SetBkMode(TRANSPARENT);

	CString msg = _T("모터 목록에서 하위 모터를 추가할 상위 모터를 선택하세요");
	CSize textSize = pDC->GetTextExtent(msg);
	pDC->TextOutW((rect.left + rect.right) / 2, rect.top + 50, msg);

	pDC->SelectObject(pOldFont);
}

void CChildView::DrawRotatingMotorShape(const Motor& motor, CDC* pDC) {
	const double PI = 3.14159265358979323846;

	CPoint screenCenter = m_motorTransform.LogicalToScreen(motor.motorPos);
	CPoint screenEnd = m_motorTransform.LogicalToScreen(motor.motorPos + CSize(motor.motorSize.cx, 0));
	int radius = (int)std::sqrt(std::pow(screenEnd.x - screenCenter.x, 2) +
		std::pow(screenEnd.y - screenCenter.y, 2));

	// 원 그리기
	CPoint screenMotorStart = m_motorTransform.LogicalToScreen(motor.motorPos - motor.motorSize);
	CPoint screenMotorEnd = m_motorTransform.LogicalToScreen(motor.motorPos + motor.motorSize);
	CRect motorRect(screenMotorStart, screenMotorEnd);
	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Ellipse(motorRect);
	pDC->SelectObject(pOldBrush);

	double angleRad = motor.rotationAngle * PI / 180.0;
	Matrix3x3 rotationMatrix = m_motorTransform.rotation(angleRad);
	POINT rotatedVec = rotationMatrix.transformPoint(radius, 0);
	CPoint end(screenCenter.x + rotatedVec.x, screenCenter.y + rotatedVec.y);

	// 사각형 크기
	CPoint rotatingPoint(end.x, end.y);
	int boxSize = motor.motorSize.cx / 2;
	int halfSize = boxSize / 2;

	// 사각형 그리기 (회전된 점 주변)
	CPoint topLeft(rotatingPoint.x - halfSize, rotatingPoint.y - halfSize);
	CPoint topRight(rotatingPoint.x + halfSize, rotatingPoint.y - halfSize);
	CPoint bottomRight(rotatingPoint.x + halfSize, rotatingPoint.y + halfSize);
	CPoint bottomLeft(rotatingPoint.x - halfSize, rotatingPoint.y + halfSize);

	pDC->MoveTo(topLeft);
	pDC->LineTo(topRight);
	pDC->LineTo(bottomRight);
	pDC->LineTo(bottomLeft);
	pDC->LineTo(topLeft);

	// 회전각 텍스트
	pDC->SetBkMode(TRANSPARENT);
	CString str;
	int lineHeight = 20;

	// 위쪽 텍스트 시작 Y 좌표
	int topTextStartY = screenCenter.y - radius - (lineHeight * 2);

	// 회전각
	str.Format(_T("회전각: %.2f°"), motor.rotationAngle);
	pDC->TextOutW(screenCenter.x, topTextStartY, str);

	// 속도
	str.Format(_T("속도: %.2f˚/s"), motor.rotationSpeed);
	pDC->TextOutW(screenCenter.x, topTextStartY + lineHeight, str);

	// ID
	str.Format(_T("ID: %d"), motor.m_id);
	pDC->TextOutW(screenCenter.x, screenCenter.y + radius + 10, str);

	if (m_motorUI.m_isAddRotatingMotorMode) {
		// 점선 펜 생성
		CPen pen(PS_DASH, 1, RGB(255, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		// 브러시는 NULL로 설정
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		pDC->Rectangle(motorRect);

		// 펜과 브러시 복원
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

void CChildView::DrawRotatingMotor(Motor* motor, CDC* pDC) {
	if (motor && pDC) {
		DrawRotatingMotorShape(*motor, pDC);
	}
}

void CChildView::DrawPreviewRotatingMotor(CDC* pDC) {
	DrawRotatingMotorShape(m_motorUI.m_previewMotor, pDC);
}


BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const double zoomStep = 0.1;
	const double minZoom = 1.0;  // 최소 배율
	const double maxZoom = 15.0;  // 최대 배율

	// 1. 마우스 포인터 기준 논리 좌표 계산
	CPoint logicalBefore = m_motorTransform.ScreenToLogical(pt);

	// 2. 줌 인/아웃
	if (zDelta > 0)
		m_motorTransform.m_zoomFactor *= (1.0 + zoomStep);  // 확대
	else
		m_motorTransform.m_zoomFactor *= (1.0 - zoomStep);  // 축소

	// 3. 줌 배율 제한
	m_motorTransform.m_zoomFactor = min(max(m_motorTransform.m_zoomFactor, minZoom), maxZoom);

	// 4. 줌 후 다시 화면 좌표 변환
	CPoint logicalAfter = m_motorTransform.ScreenToLogical(pt);

	// 5. 팬 오프셋 보정
	m_motorTransform.m_panOffset.x += (logicalAfter.x - logicalBefore.x) * static_cast<LONG>(m_motorTransform.m_zoomFactor);
	m_motorTransform.m_panOffset.y += (logicalAfter.y - logicalBefore.y) * static_cast<LONG>(m_motorTransform.m_zoomFactor);

	// 6. 줌 배율을 상태 표시줄에 표시
	CString str;
	str.Format(_T("🔍 x%.1f"), m_motorTransform.m_zoomFactor);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->m_wndStatusBar.SetPaneText(2, str);
	}

	InvalidateRect(m_drawArea, FALSE); // 영역만 갱신
	return TRUE;
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 위치 (point.x, point.y)를 비율 좌표로 변환
	CPoint logicalPoint = m_motorTransform.ScreenToLogical(point);

	// 마우스 위치를 상태 표시줄에 표시할 텍스트로 변환
	CString str;
	str.Format(_T("➕ %d, %dpx"), logicalPoint.x, logicalPoint.y);

	// CMainFrame 포인터 얻기 (정확하게 CMainFrame으로 캐스팅)
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->m_wndStatusBar.SetPaneText(1, str);
	}

	if (m_isPanning) {
		CPoint delta = point - m_lastMousePos;
		m_motorTransform.m_panOffset += delta;
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