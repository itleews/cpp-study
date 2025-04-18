
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

	// 모터 영역을 고정 크기로 설정
	CRect motorDrawArea(0, 0, 601, 601);
	dc.FillSolidRect(motorDrawArea, RGB(240, 240, 240));

	// 모터 도형 그리기 (0,0 ~ 600,600 영역 안에서만)
	for (auto axis : m_motorManager.axisList) {
		if (motorDrawArea.PtInRect(axis->strPos) && motorDrawArea.PtInRect(axis->endPos)) {
			dc.Rectangle(axis->strPos.x, axis->strPos.y - 5,
				axis->endPos.x, axis->endPos.y + 5);
			CString str;
			str.Format(_T("ID:%d"), axis->m_id);
			dc.TextOutW(axis->strPos.x, axis->strPos.y - 30, str);
		}
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
	m_motorListCtrl.InsertColumn(1, _T("축"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(2, _T("시작 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(3, _T("끝 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(4, _T("현재 위치"), LVCFMT_LEFT, 100);

	// 시작 위치 (x, y)
	m_startXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(10, 10, 60, 30), this, 2001);
	m_startXEdit.SetWindowTextW(_T("0"));

	m_startYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(70, 10, 120, 30), this, 2002);
	m_startYEdit.SetWindowTextW(_T("100"));

	// 끝 위치 (x, y)
	m_endXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(130, 10, 180, 30), this, 2003);
	m_endXEdit.SetWindowTextW(_T("100"));

	m_endYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(190, 10, 240, 30), this, 2004);
	m_endYEdit.SetWindowTextW(_T("100"));

	// 라벨 생성
	m_labelStart.Create(_T("시작"), WS_CHILD | WS_VISIBLE, CRect(10, 35, 40, 55), this);
	m_labelEnd.Create(_T("끝"), WS_CHILD | WS_VISIBLE, CRect(130, 35, 160, 55), this);
	m_labelX.Create(_T("x"), WS_CHILD | WS_VISIBLE, CRect(0, 60, 20, 80), this);
	m_labelY.Create(_T("y"), WS_CHILD | WS_VISIBLE, CRect(0, 90, 20, 110), this);

	// 라디오 버튼 생성
	m_radioXAxis.Create(_T("X축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(10, 120, 60, 140), this, 3001);
	m_radioYAxis.Create(_T("Y축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(70, 120, 120, 140), this, 3002);
	m_radioXAxis.SetCheck(BST_CHECKED); // 기본 X축 선택


	// 버튼 생성
	m_addMotorButton.Create(_T("추가"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, 1001);
	m_removeMotorButton.Create(_T("삭제"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, 1002);
	m_saveMotorButton.Create(_T("저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, 1003);
	m_loadMotorButton.Create(_T("불러오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), this, 1004);

	// 조작부
	m_motorControlStatic.Create(_T("모터 조작부"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1005);

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	const int margin = 10;
	const int sectionGap = 20;
	const int btnHeight = 30;
	const int btnGap = 10;
	const int editWidth = 50;
	const int editHeight = 20;

	int motorAreaWidth = 601;
	int motorAreaHeight = 601;

	// 왼쪽 고정 영역 크기
	int rightX = motorAreaWidth + margin;
	int rightWidth = cx - rightX - margin;

	// 리스트 컨트롤 (오른쪽 상단)
	int listHeight = cy / 3;
	m_motorListCtrl.SetWindowPos(nullptr, rightX, margin, rightWidth, listHeight, SWP_NOZORDER);

	// 📌 위치 입력 영역
	int labelTop = listHeight + 5;
	int inputTopX = labelTop + 20;
	int inputTopY = inputTopX + 30;

	// 라벨
	m_labelStart.SetWindowPos(nullptr, rightX + 20, labelTop, 30, 20, SWP_NOZORDER);
	m_labelEnd.SetWindowPos(nullptr, rightX + 120, labelTop, 30, 20, SWP_NOZORDER);
	m_labelX.SetWindowPos(nullptr, rightX, inputTopX, 20, 20, SWP_NOZORDER);
	m_labelY.SetWindowPos(nullptr, rightX, inputTopY, 20, 20, SWP_NOZORDER);

	// 입력창
	m_startXEdit.SetWindowPos(nullptr, rightX + 30, inputTopX, editWidth, editHeight, SWP_NOZORDER);
	m_endXEdit.SetWindowPos(nullptr, rightX + 130, inputTopX, editWidth, editHeight, SWP_NOZORDER);
	m_startYEdit.SetWindowPos(nullptr, rightX + 30, inputTopY, editWidth, editHeight, SWP_NOZORDER);
	m_endYEdit.SetWindowPos(nullptr, rightX + 130, inputTopY, editWidth, editHeight, SWP_NOZORDER);

	// 라디오 버튼
	int radioTop = inputTopY + editHeight + 10;
	m_radioXAxis.SetWindowPos(nullptr, rightX, radioTop, 60, 20, SWP_NOZORDER);
	m_radioYAxis.SetWindowPos(nullptr, rightX + 80, radioTop, 60, 20, SWP_NOZORDER);

	// 버튼
	int btnTop = radioTop + 30;
	int halfWidth = (rightWidth - btnGap) / 2;

	m_addMotorButton.SetWindowPos(nullptr, rightX, btnTop, halfWidth, btnHeight, SWP_NOZORDER);
	m_removeMotorButton.SetWindowPos(nullptr, rightX + halfWidth + btnGap, btnTop, halfWidth, btnHeight, SWP_NOZORDER);

	m_saveMotorButton.SetWindowPos(nullptr, rightX, btnTop + btnHeight + btnGap, halfWidth, btnHeight, SWP_NOZORDER);
	m_loadMotorButton.SetWindowPos(nullptr, rightX + halfWidth + btnGap, btnTop + btnHeight + btnGap, halfWidth, btnHeight, SWP_NOZORDER);

	// 모터 조작부
	int controlTop = btnTop + 2 * (btnHeight + btnGap) + sectionGap;
	int controlHeight = cy - controlTop - margin;
	m_motorControlStatic.SetWindowPos(nullptr, rightX, controlTop, rightWidth, controlHeight, SWP_NOZORDER);
}

void CChildView::OnAddMotor()
{
	BOOL isXSelected = m_radioXAxis.GetCheck() == BST_CHECKED;

	// 위치 텍스트 읽기
	CString sx, sy, ex, ey;
	m_startXEdit.GetWindowTextW(sx);
	m_startYEdit.GetWindowTextW(sy);
	m_endXEdit.GetWindowTextW(ex);
	m_endYEdit.GetWindowTextW(ey);

	int startX = _ttoi(sx), startY = _ttoi(sy);
	int endX = _ttoi(ex), endY = _ttoi(ey);

	// 보정
	if (isXSelected) {
		endY = startY; // X축: 수평이므로 Y 고정
	}
	else {
		endX = startX; // Y축: 수직이므로 X 고정
	}

	// 위치 제한
	startX = max(0, min(600, startX));
	startY = max(0, min(600, startY));
	endX = max(0, min(600, endX));
	endY = max(0, min(600, endY));

	m_motorManager.AddAxis(CPoint(startX, startY), CPoint(endX, endY));

	// 리스트 컨트롤에 표시
	int index = m_motorListCtrl.GetItemCount();
	CString idStr, typeStr, strPosStr, endPosStr;
    if (!m_motorManager.axisList.empty()) {
       idStr.Format(_T("%d"), m_motorManager.axisList.back()->m_id);
    }
	typeStr = isXSelected ? _T("X") : _T("Y");
	strPosStr.Format(_T("(%d, %d)"), startX, startY);
	endPosStr.Format(_T("(%d, %d)"), endX, endY);

	m_motorListCtrl.InsertItem(index, idStr);
	m_motorListCtrl.SetItemText(index, 1, typeStr);
	m_motorListCtrl.SetItemText(index, 2, strPosStr);
	m_motorListCtrl.SetItemText(index, 3, endPosStr);

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