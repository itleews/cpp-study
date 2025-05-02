
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MotorControl.h"
#include "ChildView.h"
#include <algorithm>

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
	ON_BN_CLICKED(1001, &CChildView::OnAddMotor) // 축 추가 버튼 클릭
	ON_BN_CLICKED(1002, &CChildView::OnRemoveMotor) // 축 삭제 버튼 클릭
	ON_BN_CLICKED(1003, &CChildView::OnSaveMotor) // 모터 저장 버튼 클릭
	ON_BN_CLICKED(1004, &CChildView::OnLoadMotor) // 모터 불러오기 버튼 클릭
	ON_EN_CHANGE(2001, &CChildView::OnChangeStartX)  // m_startXEdit
	ON_EN_CHANGE(2002, &CChildView::OnChangeStartY)  // m_startYEdit
	ON_BN_CLICKED(3003, &CChildView::OnBnClickedRadioXAxis) // X축 라디오 버튼 클릭
	ON_BN_CLICKED(3004, &CChildView::OnBnClickedRadioYAxis) // Y축 라디오 버튼 클릭
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
	CRect rightRect(m_drawArea.right, clientRect.top, clientRect.right, clientRect.bottom);
	memDC.FillSolidRect(rightRect, RGB(240, 240, 240));
	m_motorUI.m_motorControlPanel.Draw(&memDC);

	// 왼쪽 클리핑 설정
	memDC.IntersectClipRect(m_drawArea);

	DrawGrid(&memDC); // 그리드 그리기

	// 도형 그리기
	for (auto axis : m_motorManager.rootMotors)
	{
		// 화면 좌표로 변환
		CPoint screenStart = LogicalToScreen(axis->strPos);
		CPoint screenEnd = LogicalToScreen(axis->endPos);
		CPoint screenMotorStart = LogicalToScreen(axis->motorPos - axis->motorSize);
		CPoint screenMotorEnd = LogicalToScreen(axis->motorPos + axis->motorSize);

		memDC.MoveTo(screenStart.x, screenStart.y);
		memDC.LineTo(screenEnd.x, screenEnd.y);

		CRect motorRect(screenMotorStart.x, screenMotorStart.y, screenMotorEnd.x, screenMotorEnd.y);
		memDC.FillSolidRect(motorRect, RGB(255, 255, 255));
		memDC.FrameRect(motorRect, &CBrush(RGB(0, 0, 0)));

		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(RGB(0, 0, 0));
		memDC.SetTextAlign(TA_CENTER | TA_BASELINE);

		CString str;
		str.Format(_T("ID:%d"), axis->m_id);
		memDC.TextOutW(screenStart.x, screenStart.y - 30, str);

		// 하위 모터 그리기 (재귀적으로)
		DrawSubMotor(axis, &memDC);  // axis에서 children을 재귀적으로 처리
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

	m_motorUI.CreateUI(this);

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// 전체 창의 왼쪽 80% 영역을 도형 그리는 공간으로 사용
	m_drawArea.SetRect(10, 10, (int)(cx * 0.8) - 20, cy - 10);

	m_motorUI.SetPositionUI(m_drawArea, cx, cy);
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

void CChildView::OnAddMotor()
{
	// 위치 텍스트 읽기
	CString sx, sy, ex, ey, w, h;
	m_motorUI.m_startXEdit.GetWindowTextW(sx);
	m_motorUI.m_startYEdit.GetWindowTextW(sy);
	m_motorUI.m_endXEdit.GetWindowTextW(ex);
	m_motorUI.m_endYEdit.GetWindowTextW(ey);
	m_motorUI.m_width.GetWindowTextW(w);
	m_motorUI.m_height.GetWindowTextW(h);

	CPoint start(_ttoi(sx), _ttoi(sy));
	CPoint end(_ttoi(ex), _ttoi(ey));
	CSize motor(_ttoi(w), _ttoi(h));

	CRect curRect(start.x, start.y, end.x, end.y);
	curRect.NormalizeRect();  // 좌상단-우하단 정렬

	if (curRect.Width() > m_logicalBounds.Width() || curRect.Height() > m_logicalBounds.Height()) {
		m_logicalBounds.SetRect(0, 0, max(curRect.Width() + 10, m_logicalBounds.Width() + 10), max(curRect.Height() + 10, m_logicalBounds.Height() + 10));
	}

	int motorX = m_motorUI.m_radioXAxis.GetCheck() == BST_CHECKED ? (end.x - start.x) / 2 + start.x : end.x;
	int motorY = m_motorUI.m_radioXAxis.GetCheck() == BST_CHECKED ? start.y : (end.y - start.y) / 2 + start.y;

	// 체크박스 상태에 따라 하위 모터 추가 여부 결정
	BOOL bChecked = m_motorUI.m_subMotorCheck.GetCheck();
	Motor* parentMotor = nullptr;

	if (bChecked == BST_CHECKED) {
		// 하위 모터를 추가하려면 부모 모터를 선택해야 함
		int selectedIndex = m_motorUI.m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (selectedIndex != -1) {
			CString motorID = m_motorUI.m_motorListCtrl.GetItemText(selectedIndex, 0);
			int motorIdInt = _ttoi(motorID);
			parentMotor = GetSelectedMotor(motorIdInt);

			if (!parentMotor) {
				AfxMessageBox(_T("모터를 선택하세요!"));
				return;
			}

			// 하위 모터는 부모 모터의 영역 내에 있어야 함
			CPoint motorStart = parentMotor->motorPos - parentMotor->motorSize;
			CPoint motorEnd = parentMotor->motorPos + parentMotor->motorSize;

			// 하위 모터가 부모 모터의 영역 내에 있어야 함을 체크
			if (start.x < motorStart.x || start.y < motorStart.y ||
				end.x > motorEnd.x || end.y > motorEnd.y) {
				AfxMessageBox(_T("하위 모터는 상위 모터의 영역 내에 있어야 합니다."));
				return;
			}
		}
		else {
			AfxMessageBox(_T("하위 모터를 추가할 항목을 선택하세요!"));
			return;
		}
	}

	// 모터 추가
	m_motorManager.AddMotor(
		parentMotor, // 부모 모터 (없으면 nullptr)
		m_motorUI.m_radioXAxis.GetCheck() == BST_CHECKED,
		CPoint(start.x, start.y),
		CPoint(end.x, end.y),
		CPoint(motorX, motorY),
		CSize(motor.cx / 2, motor.cy / 2)
	);

	// 리스트 컨트롤에 모터 트리 표시
	m_motorUI.DisplayMotorTree(m_motorUI.m_motorListCtrl, m_motorManager.rootMotors);

	// 다시 그리기
	Invalidate();
}


Motor* CChildView::GetSelectedMotor(int selectedIndex)
{
	// rootMotors에서 시작하여 트리 구조를 재귀적으로 탐색
	for (Motor* rootMotor : m_motorManager.rootMotors)
	{
		Motor* selectedMotor = FindMotorByID(rootMotor, selectedIndex);
		if (selectedMotor) return selectedMotor;
	}
	return nullptr;
}

// 재귀적으로 모터를 찾는 함수 (ID 비교)
Motor* CChildView::FindMotorByID(Motor* node, int selectedID)
{
	if (!node) return nullptr;

	if (node->m_id == selectedID)  // 선택된 ID와 일치하면 그 모터를 반환
		return node;

	// 자식들에 대해 재귀 호출
	for (Motor* child : node->children)
	{
		Motor* foundMotor = FindMotorByID(child, selectedID);
		if (foundMotor) return foundMotor;
	}

	return nullptr; // 찾지 못했으면 null 반환
}



// 하위 모터를 재귀적으로 그리기 위한 함수
void CChildView::DrawSubMotor(Motor* parentMotor, CDC* pDC)
{
	// 하위 모터가 있는 경우 재귀적으로 그리기
	for (Motor* child : parentMotor->children)
	{
		// 화면 좌표로 변환
		CPoint screenStart = LogicalToScreen(child->strPos);
		CPoint screenEnd = LogicalToScreen(child->endPos);
		CPoint screenMotorStart = LogicalToScreen(child->motorPos - child->motorSize);
		CPoint screenMotorEnd = LogicalToScreen(child->motorPos + child->motorSize);

		pDC->MoveTo(screenStart.x, screenStart.y);
		pDC->LineTo(screenEnd.x, screenEnd.y);

		CRect motorRect(screenMotorStart.x, screenMotorStart.y, screenMotorEnd.x, screenMotorEnd.y);
		pDC->FillSolidRect(motorRect, RGB(255, 255, 255));
		pDC->FrameRect(motorRect, &CBrush(RGB(0, 0, 0)));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetTextAlign(TA_CENTER | TA_BASELINE);

		CString str;
		str.Format(_T("ID:%d"), child->m_id);
		pDC->TextOutW(screenStart.x, screenStart.y - 30, str);

		// 자식의 하위 모터도 그리기 (재귀 호출)
		DrawSubMotor(child, pDC);
	}
}

void CChildView::OnRemoveMotor()
{
	std::vector<int> selectedIndices;

	// 다중 선택된 인덱스 수집
	int index = -1;
	while ((index = m_motorUI.m_motorListCtrl.GetNextItem(index, LVNI_SELECTED)) != -1)
	{
		selectedIndices.push_back(index);
	}

	if (selectedIndices.empty())
	{
		AfxMessageBox(_T("삭제할 모터를 선택하세요!"));
		return;
	}

	// 삭제할 Motor 포인터 저장
	std::vector<Motor*> motorsToDelete;

	for (int selectedIndex : selectedIndices)
	{
		CString motorIDStr = m_motorUI.m_motorListCtrl.GetItemText(selectedIndex, 0);
		int motorIdInt = _ttoi(motorIDStr);

		Motor* motorToDelete = GetSelectedMotor(motorIdInt);
		if (motorToDelete)
		{
			motorsToDelete.push_back(motorToDelete);
		}
	}

	// 삭제 처리
	for (Motor* motorToDelete : motorsToDelete)
	{
		if (motorToDelete->parentMotor)
		{
			auto& siblings = motorToDelete->parentMotor->subMotors;
			auto it = std::find(siblings.begin(), siblings.end(), motorToDelete);
			if (it != siblings.end())
				siblings.erase(it);
		}
		else
		{
			auto& roots = m_motorManager.rootMotors;
			auto it = std::find(roots.begin(), roots.end(), motorToDelete);
			if (it != roots.end())
				roots.erase(it);
		}

		delete motorToDelete;
	}

	// 인덱스 역순으로 리스트에서 삭제 (index shift 방지)
	std::sort(selectedIndices.rbegin(), selectedIndices.rend());
	for (int selectedIndex : selectedIndices)
	{
		m_motorUI.m_motorListCtrl.DeleteItem(selectedIndex);
	}

	// 다시 그리기
	Invalidate();
}


// 모터 저장 기능
void CChildView::OnSaveMotor()
{
	m_motorManager.SaveMotorData();
}

void CChildView::OnLoadMotor()
{
	m_motorManager.LoadMotorData();
	// 리스트 컨트롤 초기화
	m_motorUI.m_motorListCtrl.DeleteAllItems();

	// 모터 리스트에 있는 모든 모터를 리스트 컨트롤에 추가
	for (auto motor : m_motorManager.rootMotors)
	{
		int index = m_motorUI.m_motorListCtrl.GetItemCount();
		CString idStr, typeStr, strPosStr, endPosStr, motorPosStr;
		idStr.Format(_T("%d"), motor->m_id);
		typeStr = motor->isX ? _T("X") : _T("Y");
		strPosStr.Format(_T("(%d, %d)"), motor->strPos.x, motor->strPos.y);
		endPosStr.Format(_T("(%d, %d)"), motor->endPos.x, motor->endPos.y);
		motorPosStr.Format(_T("(%d, %d)"), motor->motorPos.x, motor->motorPos.y);
		m_motorUI.m_motorListCtrl.InsertItem(index, idStr);
		m_motorUI.m_motorListCtrl.SetItemText(index, 1, typeStr);
		m_motorUI.m_motorListCtrl.SetItemText(index, 2, strPosStr);
		m_motorUI.m_motorListCtrl.SetItemText(index, 3, endPosStr);
		m_motorUI.m_motorListCtrl.SetItemText(index, 4, motorPosStr);
	}

	Invalidate();
}

void CChildView::OnBnClickedRadioXAxis()
{
	m_motorUI.m_endYEdit.EnableWindow(FALSE);
	m_motorUI.m_endXEdit.EnableWindow(TRUE);
	CString startY;
	m_motorUI.m_startYEdit.GetWindowTextW(startY);
	m_motorUI.m_endYEdit.SetWindowTextW(startY);
}

void CChildView::OnBnClickedRadioYAxis()
{
	m_motorUI.m_endXEdit.EnableWindow(FALSE);
	m_motorUI.m_endYEdit.EnableWindow(TRUE);
	CString startX;
	m_motorUI.m_startXEdit.GetWindowTextW(startX);
	m_motorUI.m_endXEdit.SetWindowTextW(startX);
}

void CChildView::OnChangeStartX()
{
	if (m_motorUI.m_radioYAxis.GetSafeHwnd() && m_motorUI.m_radioYAxis.GetCheck() == BST_CHECKED)  // Y축 라디오 버튼 선택되었을 때
	{
		CString startX;
		m_motorUI.m_startXEdit.GetWindowTextW(startX);
		m_motorUI.m_endXEdit.SetWindowTextW(startX);
	}
}

void CChildView::OnChangeStartY()
{
	if (m_motorUI.m_radioXAxis.GetSafeHwnd() && m_motorUI.m_radioXAxis.GetCheck() == BST_CHECKED)  // X축 라디오 버튼 선택되었을 때
	{
		CString startY;
		m_motorUI.m_startYEdit.GetWindowTextW(startY);
		m_motorUI.m_endYEdit.SetWindowTextW(startY);
	}
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

// CChildView 클래스 내에 OnMouseMove 함수 추가

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