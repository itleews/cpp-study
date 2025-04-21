
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MotorControl.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	ON_BN_CLICKED(3002, &CChildView::OnBnClickedRadioXAxis) // X축 라디오 버튼 클릭
	ON_BN_CLICKED(3003, &CChildView::OnBnClickedRadioYAxis) // Y축 라디오 버튼 클릭
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

	// 배경 그리기
	dc.FillSolidRect(m_drawArea, RGB(255, 255, 255));
	CRect rightRect(m_drawArea.right, clientRect.top, clientRect.right, clientRect.bottom);
	dc.FillSolidRect(rightRect, RGB(240, 240, 240));
	dc.FrameRect(rightRect, &CBrush(RGB(0, 0, 0)));

	double baseWidth = max((double)m_baseRect.Width(), 1.0);
	double baseHeight = max((double)m_baseRect.Height(), 1.0);

	for (auto axis : m_motorManager.axisList)
	{
		// 비율 -> 실제 좌표로 환산
		int strX = (int)(axis->strXRatio * m_drawArea.Width()) + m_drawArea.left;
		int strY = (int)(axis->strYRatio * m_drawArea.Height()) + m_drawArea.top;
		int endX = (int)(axis->endXRatio * m_drawArea.Width()) + m_drawArea.left;
		int endY = (int)(axis->endYRatio * m_drawArea.Height()) + m_drawArea.top;
		int motorX = (int)(axis->motorXRatio * m_drawArea.Width()) + m_drawArea.left;
		int motorY = (int)(axis->motorYRatio * m_drawArea.Height()) + m_drawArea.top;

		// 선 그리기
		dc.MoveTo(strX, strY);
		dc.LineTo(endX, endY);

		// 모터 사각형
		CRect motorRect(
			motorX - axis->motorSize.cx / 2,
			motorY - axis->motorSize.cy / 2,
			motorX + axis->motorSize.cx / 2,
			motorY + axis->motorSize.cy / 2
		);
		dc.FillSolidRect(motorRect, RGB(255, 255, 255));
		dc.FrameRect(motorRect, &CBrush(RGB(0, 0, 0)));

		// 텍스트
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetTextAlign(TA_CENTER | TA_BASELINE);

		CString str;
		str.Format(_T("ID:%d"), axis->m_id);
		dc.TextOutW(strX, strY - 30, str);
	}
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 리스트 컨트롤 생성
	m_motorListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 0, 0), this, 1);
	m_motorListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_motorListCtrl.InsertColumn(0, _T("모터 ID"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(1, _T("기준 축"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(2, _T("시작 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(3, _T("끝 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(4, _T("현재 위치"), LVCFMT_LEFT, 100);

	m_groupInput.Create(_T("위치 및 크기 설정"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3000);
	m_groupButtons.Create(_T("모터 관리"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3001);

	// 시작 위치 (X, Y)
	m_startXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2001);
	m_startXEdit.SetWindowTextW(_T("0"));

	m_startYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2002);
	m_startYEdit.SetWindowTextW(_T("100"));

	// 끝 위치 (X, Y)
	m_endXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2003);
	m_endXEdit.SetWindowTextW(_T("100"));

	m_endYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2004);
	m_endYEdit.SetWindowTextW(_T("100"));

	// 크기
	m_width.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2005);
	m_width.SetWindowTextW(_T("50"));

	m_height.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2006);
	m_height.SetWindowTextW(_T("50"));

	// 축 선택 라디오 버튼
	m_radioXAxis.Create(_T("X축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3002);
	m_radioYAxis.Create(_T("Y축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3003);
	m_radioXAxis.SetCheck(TRUE); // 기본으로 X축 선택
	m_endYEdit.EnableWindow(FALSE); // Y축 비활성화

	// 라벨
	m_labelStart.Create(_T("시작 위치(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_labelEnd.Create(_T("끝 위치(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_labelSize.Create(_T("크기(W, H)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_labelAxis.Create(_T("기준 축"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);

	// 모터 관리 버튼
	m_addMotorButton.Create(_T("추가"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1001);
	m_removeMotorButton.Create(_T("삭제"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1002);
	m_saveMotorButton.Create(_T("저장"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1003);
	m_loadMotorButton.Create(_T("불러오기"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1004);

	// 조작부
	m_motorControlStatic.Create(_T("모터 조작부"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1005);

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// 전체 창의 왼쪽 60% 영역을 도형 그리는 공간으로 사용
	m_drawArea.SetRect(10, 10, (int)(cx * 0.8) - 10, cy - 10);

	const int margin = 10;
	const int sectionGap = 20;

	const int btnHeight = 30;
	const int btnGap = 10;

	const int editWidth = 100;
	const int editHeight = 25;

	int rightWidth = cx - m_drawArea.right - 2 * margin;
	int motorAreaWidth = cx - rightWidth - 2 * margin;
	int motorAreaHeight = cy - 2 * margin;

	int rightX = motorAreaWidth + margin;

	// 1. 리스트 컨트롤
	int listHeight = cy / 3;
	m_motorListCtrl.SetWindowPos(nullptr, rightX, margin, rightWidth, listHeight, SWP_NOZORDER);

	int inputStartX = rightX + 10;
	int inputStartY = listHeight + sectionGap;
	int inputGroupWidth = rightWidth;

	const int rowGap = 10;
	const int colGap = 10;
	const int labelWidth = 150;
	const int fieldWidth = 80;
	const int fieldHeight = 25;
	const int groupPadding = 10;

	int halfWidth = (inputGroupWidth - groupPadding * 2 - colGap) / 2;

	// 2. 입력 그룹박스
	int row1Y = inputStartY + groupPadding + 15;
	m_labelStart.SetWindowPos(nullptr, inputStartX + groupPadding, row1Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_startXEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row1Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_startYEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row1Y, fieldWidth, fieldHeight, SWP_NOZORDER);

	int row2Y = row1Y + fieldHeight + rowGap;
	m_labelEnd.SetWindowPos(nullptr, inputStartX + groupPadding, row2Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_endXEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_endYEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);

	int row3Y = row2Y + fieldHeight + rowGap;
	m_labelSize.SetWindowPos(nullptr, inputStartX + groupPadding, row3Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_width.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_height.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);

	int row4Y = row3Y + fieldHeight + rowGap;
	m_labelAxis.SetWindowPos(nullptr, inputStartX + groupPadding, row4Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_radioXAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row4Y, 60, fieldHeight, SWP_NOZORDER);
	m_radioYAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + 70, row4Y, 60, fieldHeight, SWP_NOZORDER);

	int inputGroupHeight = (row4Y + fieldHeight + groupPadding) - inputStartY;
	m_groupInput.SetWindowPos(nullptr, inputStartX, inputStartY, inputGroupWidth, inputGroupHeight, SWP_NOZORDER);

	// 3. 버튼 그룹박스
	int buttonGroupY = inputStartY + inputGroupHeight + sectionGap;

	int buttonY = buttonGroupY + 20;
	m_addMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY, halfWidth, btnHeight, SWP_NOZORDER);
	m_removeMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY, halfWidth, btnHeight, SWP_NOZORDER);

	int buttonY2 = buttonY + btnHeight + rowGap;
	m_saveMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY2, halfWidth, btnHeight, SWP_NOZORDER);
	m_loadMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY2, halfWidth, btnHeight, SWP_NOZORDER);

	int buttonGroupHeight = (buttonY2 + btnHeight + groupPadding) - buttonGroupY;
	m_groupButtons.SetWindowPos(nullptr, inputStartX, buttonGroupY, inputGroupWidth, buttonGroupHeight, SWP_NOZORDER);

	// 4. 조작부
	int controlTop = buttonGroupY + buttonGroupHeight + sectionGap;
	int controlHeight = cy - controlTop - margin;
	m_motorControlStatic.SetWindowPos(nullptr, rightX, controlTop, rightWidth, controlHeight, SWP_NOZORDER);
}


void CChildView::OnAddMotor()
{
	BOOL isXSelected = m_radioXAxis.GetCheck() == BST_CHECKED;

	// 위치 텍스트 읽기
	CString sx, sy, ex, ey, w, h;
	m_startXEdit.GetWindowTextW(sx);
	m_startYEdit.GetWindowTextW(sy);
	m_endXEdit.GetWindowTextW(ex);
	m_endYEdit.GetWindowTextW(ey);
	m_width.GetWindowTextW(w);
	m_height.GetWindowTextW(h);

	int startX = _ttoi(sx), startY = _ttoi(sy);
	int endX = _ttoi(ex), endY = _ttoi(ey);
	int width = _ttoi(w), height = _ttoi(h);

	CRect curRect(startX, startY, endX, endY);
	curRect.NormalizeRect();  // 좌상단-우하단 정렬

	// 크기가 늘어나면 m_baseRect를 갱신
	if (curRect.Width() > m_baseRect.Width() || curRect.Height() > m_baseRect.Height()) {
		m_motorManager.prevBaseRect = m_motorManager.baseRect;

		m_baseRect.SetRect(0, 0, max(curRect.Width() + 10, m_baseRect.Width() + 10), max(curRect.Height() + 10, m_baseRect.Height() + 10));
		m_motorManager.UpdateAllMotorRatios();
	}

	// 비례 계산을 위한 값
	double baseWidth = max((double)m_baseRect.Width(), 1.0);
	double baseHeight = max((double)m_baseRect.Height(), 1.0);

	// 좌표를 비율로 변환
	double strXRatio = (startX - m_baseRect.left) / baseWidth;
	double strYRatio = (startY - m_baseRect.top) / baseHeight;
	double endXRatio = (endX - m_baseRect.left) / baseWidth;
	double endYRatio = (endY - m_baseRect.top) / baseHeight;

	// 모터 위치도 비율로 변환
	int motorX = isXSelected ? (endX - startX) / 2 + startX : endX;
	int motorY = isXSelected ? startY : (endY - startY) / 2 + startY;
	double motorXRatio = (motorX - m_baseRect.left) / baseWidth;
	double motorYRatio = (motorY - m_baseRect.top) / baseHeight;

	// 변경된 AddAxis 호출
	m_motorManager.AddAxis(
		isXSelected,
		strXRatio, strYRatio,
		endXRatio, endYRatio,
		motorXRatio, motorYRatio,
		CSize(width, height)
	);

	// 리스트 컨트롤에 표시
	int index = m_motorListCtrl.GetItemCount();
	CString idStr, typeStr, strPosStr, endPosStr, motorPosStr;
    if (!m_motorManager.axisList.empty()) {
       idStr.Format(_T("%d"), m_motorManager.axisList.back()->m_id);
    }
	typeStr = isXSelected ? _T("X") : _T("Y");
	strPosStr.Format(_T("(%d, %d)"), startX, startY);
	endPosStr.Format(_T("(%d, %d)"), endX, endY);
	motorPosStr.Format(_T("(%d, %d)"), motorX, motorY);

	m_motorListCtrl.InsertItem(index, idStr);
	m_motorListCtrl.SetItemText(index, 1, typeStr);
	m_motorListCtrl.SetItemText(index, 2, strPosStr);
	m_motorListCtrl.SetItemText(index, 3, endPosStr);
	m_motorListCtrl.SetItemText(index, 4, motorPosStr);

	// 다시 그리기
	Invalidate();
}

void CChildView::OnRemoveMotor()
{
	// 선택된 항목 인덱스를 가져옴
	int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);

	if (selectedIndex != -1)
	{
		// 리스트에서 선택된 항목 제거
		m_motorListCtrl.DeleteItem(selectedIndex);

		// axisList에서도 해당 ID에 해당하는 축 제거
		if (selectedIndex >= 0 && selectedIndex < m_motorManager.axisList.size())
		{
			// 메모리 해제 후 제거
			delete m_motorManager.axisList[selectedIndex];
			m_motorManager.axisList.erase(m_motorManager.axisList.begin() + selectedIndex);
		}

		// 다시 그리기
		Invalidate();
	}
	else
	{
		AfxMessageBox(_T("삭제할 항목을 선택하세요!"));
	}
}


void CChildView::OnSaveMotor()
{
	AfxMessageBox(_T("모터 저장 기능 실행 (파일 저장 로직 구현 예정)"));
}

void CChildView::OnLoadMotor()
{
	AfxMessageBox(_T("모터 불러오기 기능 실행 (파일 로딩 로직 구현 예정)"));
}

void CChildView::OnBnClickedRadioXAxis()
{
	m_endYEdit.EnableWindow(FALSE);
	m_endXEdit.EnableWindow(TRUE);
	CString startY;
	m_startYEdit.GetWindowTextW(startY);
	m_endYEdit.SetWindowTextW(startY);
}

void CChildView::OnBnClickedRadioYAxis()
{
	m_endXEdit.EnableWindow(FALSE);
	m_endYEdit.EnableWindow(TRUE);
	CString startX;
	m_startXEdit.GetWindowTextW(startX);
	m_endXEdit.SetWindowTextW(startX);
}

void CChildView::OnChangeStartX()
{
	if (m_radioYAxis.GetSafeHwnd() && m_radioYAxis.GetCheck() == BST_CHECKED)  // Y축 라디오 버튼 선택되었을 때
	{
		CString startX;
		m_startXEdit.GetWindowTextW(startX);
		m_endXEdit.SetWindowTextW(startX);
	}
}

void CChildView::OnChangeStartY()
{
	if (m_radioXAxis.GetSafeHwnd() && m_radioXAxis.GetCheck() == BST_CHECKED)  // X축 라디오 버튼 선택되었을 때
	{
		CString startY;
		m_startYEdit.GetWindowTextW(startY);
		m_endYEdit.SetWindowTextW(startY);
	}
}
