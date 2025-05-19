#include "pch.h"
#include "MotorUI.h"
#include "ChildView.h"
#include <algorithm>

BEGIN_MESSAGE_MAP(MotorUI, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(1001, &MotorUI::OnAddMotor) // 축 추가 버튼 클릭
	ON_BN_CLICKED(2007, &MotorUI::OnAddSubMotor) // 하위 모터 추가 버튼 클릭
	ON_BN_CLICKED(1002, &MotorUI::OnRemoveMotor) // 축 삭제 버튼 클릭
	ON_BN_CLICKED(1003, &MotorUI::OnSaveMotor) // 모터 저장 버튼 클릭
	ON_BN_CLICKED(1004, &MotorUI::OnLoadMotor) // 모터 불러오기 버튼 클릭
	ON_EN_CHANGE(2001, &MotorUI::OnChangeStartX)  // m_startXEdit
	ON_EN_CHANGE(2002, &MotorUI::OnChangeStartY)  // m_startYEdit
	ON_BN_CLICKED(3003, &MotorUI::OnBnClickedRadioXAxis) // X축 라디오 버튼 클릭
	ON_BN_CLICKED(3004, &MotorUI::OnBnClickedRadioYAxis) // Y축 라디오 버튼 클릭
	ON_NOTIFY(LVN_ITEMCHANGED, 1, &MotorUI::OnLvnItemChangedMotorList) // 리스트 컨트롤 항목 변경
END_MESSAGE_MAP()

int MotorUI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    // 리스트 컨트롤 생성
    m_motorListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 0, 0), this, 1);
    m_motorListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER);
    m_motorListCtrl.InsertColumn(0, _T("모터 ID"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(1, _T("축"), LVCFMT_LEFT, 50);
    m_motorListCtrl.InsertColumn(2, _T("시작 위치"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(3, _T("끝 위치"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(4, _T("현재 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(5, _T("절대 위치"), LVCFMT_LEFT, 100);

    // 그룹박스 생성
    m_groupInput.Create(_T("모터 관리"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3000);
    m_groupButtons.Create(_T("파일 관리"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3001);
	m_groupControl.Create(_T("모터 조작부"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3002);

    // 입력 필드 생성
    m_startXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2001);
    m_startXEdit.SetWindowTextW(_T("0"));

    m_startYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2002);
    m_startYEdit.SetWindowTextW(_T("100"));

    m_endXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2003);
    m_endXEdit.SetWindowTextW(_T("1000"));

    m_endYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2004);
    m_endYEdit.SetWindowTextW(_T("100"));

    m_width.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2005);
    m_width.SetWindowTextW(_T("100"));

    m_height.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2006);
    m_height.SetWindowTextW(_T("100"));

	m_speed.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, 2008);
	m_speed.SetWindowTextW(_T("100"));

	m_addSubMotorButton.Create(_T("하위 모터 추가"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2007);

    // 라디오 버튼 생성
    m_radioXAxis.Create(_T("X축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3003);
    m_radioYAxis.Create(_T("Y축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3004);
    m_radioXAxis.SetCheck(TRUE);  // 기본으로 X축 선택
    m_endYEdit.EnableWindow(FALSE);  // Y축 비활성화

    // 라벨 생성
    m_labelStart.Create(_T("시작 위치(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelEnd.Create(_T("끝 위치(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelSize.Create(_T("크기(W, H)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelAxis.Create(_T("기준 축"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_labelSpeed.Create(_T("모터 속도"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);

    // 모터 관리 버튼 생성
    m_addMotorButton.Create(_T("추가"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1001);
    m_removeMotorButton.Create(_T("삭제"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1002);
    m_saveMotorButton.Create(_T("저장"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1003);
    m_loadMotorButton.Create(_T("불러오기"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1004);

	// 조작부 버튼 생성
	m_controlButton[0].Create(_T("▲"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4001);
	m_controlButton[1].Create(_T("▼"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4002);
	m_controlButton[2].Create(_T("◀"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4003);
	m_controlButton[3].Create(_T("▶"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4004);

	return 0;
}

void MotorUI::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	const int margin = 10;
	const int sectionGap = 20;

	const int btnHeight = 30;
	const int btnGap = 10;

	const int editWidth = 100;
	const int editHeight = 25;

	int rightWidth = cx;
	int motorAreaWidth = cx - rightWidth - 2 * margin;
	int motorAreaHeight = cy - 2 * margin;

	int rightX = motorAreaWidth + margin + 10;

	// 1. 리스트 컨트롤
	int listHeight = cy / 3;
	m_motorListCtrl.SetWindowPos(nullptr, rightX, margin, rightWidth, listHeight, SWP_NOZORDER);

	int inputStartX = rightX + 10;
	int inputStartY = listHeight + sectionGap;
	int inputGroupWidth = rightWidth;

	const int rowGap = 10;
	const int colGap = 10;
	const int labelWidth = 170;
	const int fieldWidth = 80;
	const int fieldHeight = 25;
	const int groupPadding = 10;

	int halfWidth = (inputGroupWidth - groupPadding * 2 - colGap) / 2 - 10;

	// 2. 입력 그룹박스
	int row1Y = inputStartY + groupPadding + 15;
	m_labelStart.SetWindowPos(nullptr, inputStartX + groupPadding, row1Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_startXEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row1Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_startYEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row1Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_addMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row1Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row2Y = row1Y + fieldHeight + rowGap;
	m_labelEnd.SetWindowPos(nullptr, inputStartX + groupPadding, row2Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_endXEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_endYEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_removeMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row2Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row3Y = row2Y + fieldHeight + rowGap;
	m_labelSize.SetWindowPos(nullptr, inputStartX + groupPadding, row3Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_width.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_height.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);

	int row4Y = row3Y + fieldHeight + rowGap;
	m_labelAxis.SetWindowPos(nullptr, inputStartX + groupPadding, row4Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_radioXAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row4Y, 60, fieldHeight, SWP_NOZORDER);
	m_radioYAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + 70, row4Y, 60, fieldHeight, SWP_NOZORDER);
	
	int row5Y = row4Y + fieldHeight + rowGap;
	m_labelSpeed.SetWindowPos(nullptr, inputStartX + groupPadding, row5Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_speed.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row5Y, fieldWidth * 2 + colGap, fieldHeight, SWP_NOZORDER);
	m_addSubMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row5Y, labelWidth, btnHeight, SWP_NOZORDER);

	int inputGroupHeight = (row5Y + fieldHeight + groupPadding) - inputStartY;
	m_groupInput.SetWindowPos(nullptr, rightX, inputStartY, rightWidth, inputGroupHeight, SWP_NOZORDER);

	// 3. 버튼 그룹박스
	int buttonGroupY = inputStartY + inputGroupHeight + sectionGap;

	int buttonY = buttonGroupY + btnHeight + rowGap;
	m_saveMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY, halfWidth, btnHeight, SWP_NOZORDER);
	m_loadMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY, halfWidth, btnHeight, SWP_NOZORDER);

	int buttonGroupHeight = (buttonY + btnHeight + groupPadding) - buttonGroupY;
	m_groupButtons.SetWindowPos(nullptr, rightX, buttonGroupY, rightWidth, buttonGroupHeight, SWP_NOZORDER);

	// 4. 조작부
	int controlTop = buttonGroupY + buttonGroupHeight + sectionGap;
	int controlHeight = cy - controlTop - margin;

	int controlButtonWidth = (rightWidth - 3 * colGap) / 4;
	int controlButtonHeight = (controlHeight - 3 * rowGap) / 4;

	// 버튼 배치의 중심점
	int centerX = rightX + rightWidth / 2;
	int centerY = controlTop + controlHeight / 3;

	int bw = controlButtonWidth;
	int bh = controlButtonHeight;

	// 위 (0) 버튼 위치
	m_controlButton[0].SetWindowPos(nullptr, centerX - bw / 2, centerY - bh - rowGap, bw, bh, SWP_NOZORDER);

	// 아래 (1) 버튼 위치
	m_controlButton[1].SetWindowPos(nullptr, centerX - bw / 2, centerY + bh + rowGap, bw, bh, SWP_NOZORDER);

	// 왼 (2) 버튼 위치
	m_controlButton[2].SetWindowPos(nullptr, centerX - bw * 2 + colGap + 50, centerY, bw, bh, SWP_NOZORDER);

	// 오 (3) 버튼 위치
	m_controlButton[3].SetWindowPos(nullptr, centerX + bw / 2 + colGap, centerY, bw, bh, SWP_NOZORDER);

	// 그룹 전체의 크기 설정
	m_groupControl.SetWindowPos(nullptr, rightX, controlTop, rightWidth, controlHeight, SWP_NOZORDER);

}

void MotorUI::DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors)
{
	listCtrl.DeleteAllItems(); // 기존 내용 비우기

	int itemIndex = 0;

	for (Motor* root : rootMotors)
	{
		// 최상위 모터는 전체 좌표계 기준이므로 (0, 0) 기준으로 시작
		DisplayMotorRecursive(listCtrl, root, 0, itemIndex, CPoint(0, 0));
	}
}

void MotorUI::DisplayMotorRecursive(CListCtrl& listCtrl, Motor* node, int depth, int& itemIndex, const CPoint& parentOrigin)
{
	if (!node) return;

	CString displayMotorID;

	// 들여쓰기
	for (int i = 0; i < depth; ++i)
		displayMotorID += _T("    ");

	CString idStr;
	idStr.Format(_T("%d"), node->m_id);
	displayMotorID += idStr;

	CString typeStr, strPosStr, endPosStr, motorPosStr, absMotorPosStr;
	typeStr = node->isX ? _T("X") : _T("Y");

	// 부모 기준 상대 좌표로 출력
	CPoint relStrPos = node->strPos - parentOrigin;
	CPoint relEndPos = node->endPos - parentOrigin;
	CPoint relMotorPos = node->motorPos - parentOrigin;

	strPosStr.Format(_T("(%d, %d)"), relStrPos.x, relStrPos.y);
	endPosStr.Format(_T("(%d, %d)"), relEndPos.x, relEndPos.y);
	motorPosStr.Format(_T("(%d, %d)"), relMotorPos.x, relMotorPos.y);
	absMotorPosStr.Format(_T("(%d, %d)"), node->motorPos.x, node->motorPos.y);


	listCtrl.InsertItem(itemIndex, displayMotorID);
	listCtrl.SetItemText(itemIndex, 1, typeStr);
	listCtrl.SetItemText(itemIndex, 2, strPosStr);
	listCtrl.SetItemText(itemIndex, 3, endPosStr);
	listCtrl.SetItemText(itemIndex, 4, motorPosStr);
	listCtrl.SetItemText(itemIndex, 5, absMotorPosStr);

	++itemIndex;

	// 재귀 호출할 때 현재 모터 위치를 기준점으로 넘김
	for (Motor* child : node->children)
	{
		if (child)
			DisplayMotorRecursive(listCtrl, child, depth + 1, itemIndex, node->motorPos - node->motorSize);
	}
}


void MotorUI::OnAddMotor()
{
	// 위치 텍스트 읽기
	CString sx, sy, ex, ey, w, h, s;
	m_startXEdit.GetWindowTextW(sx);
	m_startYEdit.GetWindowTextW(sy);
	m_endXEdit.GetWindowTextW(ex);
	m_endYEdit.GetWindowTextW(ey);
	m_width.GetWindowTextW(w);
	m_height.GetWindowTextW(h);
	m_speed.GetWindowTextW(s);

	CPoint start(_ttoi(sx), _ttoi(sy));
	CPoint end(_ttoi(ex), _ttoi(ey));
	CSize motor(_ttoi(w), _ttoi(h));
	int speed = _ttoi(s);

	CRect curRect(start.x, start.y, end.x, end.y);
	curRect.NormalizeRect();  // 좌상단-우하단 정렬

	/*if (curRect.Width() > m_logicalBounds.Width() || curRect.Height() > m_logicalBounds.Height()) {
		m_logicalBounds.SetRect(0, 0, max(curRect.Width() + 10, m_logicalBounds.Width() + 10), max(curRect.Height() + 10, m_logicalBounds.Height() + 10));
	}*/

	// 하위 모터를 추가하려면 부모 모터를 선택해야 함
	BOOL bChecked = m_addSubMotorButton.GetCheck();
	Motor* parentMotor = nullptr;

	if (m_isAddSubmotorMode) {
		int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (selectedIndex != -1) {
			CString motorID = m_motorListCtrl.GetItemText(selectedIndex, 0);
			int motorIdInt = _ttoi(motorID);
			parentMotor = GetSelectedMotor(motorIdInt);

			if (!parentMotor) {
				AfxMessageBox(_T("선택한 ID에 해당하는 모터가 존재하지 않습니다."));
				return;
			}

			// 하위 모터는 부모 모터의 영역 내에 있어야 함
			CPoint motorStart = parentMotor->motorPos - parentMotor->motorSize;
			CPoint motorEnd = parentMotor->motorPos + parentMotor->motorSize;

			CPoint subMotorStart = CPoint(start.x, start.y);
			CPoint subMotorEnd = CPoint(end.x, end.y);
			start = SubToLogical(subMotorStart, motorStart);
			end = SubToLogical(subMotorEnd, motorStart);

			// 하위 모터가 부모 모터의 영역 내에 있어야 함을 체크
			if (start.x < motorStart.x || start.y < motorStart.y ||
				end.x > motorEnd.x || end.y > motorEnd.y) {
				AfxMessageBox(_T("하위 모터는 상위 모터의 영역 내에 있어야 합니다."));
				return;
			}

			m_isAddSubmotorMode = false; // 하위 모터 추가 후 모드 해제
			m_groupInput.SetWindowText(_T("모터 관리"));
			m_removeMotorButton.SetWindowText(_T("삭제"));
			m_addSubMotorButton.EnableWindow(TRUE);
		}
		else {
			AfxMessageBox(_T("하위 모터를 추가할 항목을 선택하세요!"));
			return;
		}
	}

	int motorX = m_radioXAxis.GetCheck() == BST_CHECKED ? (end.x - start.x) / 2 + start.x : end.x;
	int motorY = m_radioXAxis.GetCheck() == BST_CHECKED ? start.y : (end.y - start.y) / 2 + start.y;

	// 모터 추가
	m_motorManager.AddMotor(
		parentMotor, // 부모 모터 (없으면 nullptr)
		m_radioXAxis.GetCheck() == BST_CHECKED,
		CPoint(start.x, start.y),
		CPoint(end.x, end.y),
		CPoint(motorX, motorY),
		CSize(motor.cx / 2, motor.cy / 2),
		speed // 속도
	);

	// 리스트 컨트롤에 모터 트리 표시
	DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);

	// 다시 그리기
	if (m_pParentView)
		m_pParentView->Invalidate();
}

// 하위 모터 추가
void MotorUI::OnAddSubMotor() {
	// 하위 모터 추가 모드
	if (m_motorManager.rootMotors.empty()) {
		AfxMessageBox(_T("하위 모터를 추가할 상위 모터가 없습니다."));
		return;
	}
	m_isAddSubmotorMode = true;
	m_groupInput.SetWindowText(_T("하위 모터 추가"));
	m_removeMotorButton.SetWindowText(_T("취소"));
	m_addSubMotorButton.EnableWindow(FALSE);

	int selectedIndex = NULL;
}


Motor* MotorUI::GetSelectedMotor(int selectedIndex)
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
Motor* MotorUI::FindMotorByID(Motor* node, int selectedID)
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

void MotorUI::OnRemoveMotor()
{
	if (m_isAddSubmotorMode) {
		m_isAddSubmotorMode = false; // 하위 모터 추가 모드 해제
		m_groupInput.SetWindowText(_T("모터 관리"));
		m_removeMotorButton.SetWindowText(_T("삭제"));
		m_addSubMotorButton.EnableWindow(TRUE);
		return;
	}

	std::vector<int> selectedIndices;

	// 선택된 항목 수집
	int index = -1;
	while ((index = m_motorListCtrl.GetNextItem(index, LVNI_SELECTED)) != -1)
	{
		selectedIndices.push_back(index);
	}

	if (selectedIndices.empty())
	{
		AfxMessageBox(_T("삭제할 모터를 선택하세요!"));
		return;
	}

	// 삭제 대상 수집
	std::vector<Motor*> motorsToDelete;
	for (int selectedIndex : selectedIndices)
	{
		CString motorIDStr = m_motorListCtrl.GetItemText(selectedIndex, 0);
		int motorIdInt = _ttoi(motorIDStr);
		Motor* motorToDelete = GetSelectedMotor(motorIdInt);
		if (motorToDelete)
		{
			motorsToDelete.push_back(motorToDelete);
		}
	}

	// 삭제 수행
	for (Motor* motorToDelete : motorsToDelete)
	{
		DeleteMotorRecursive(motorToDelete); // 하위 모터 포함 삭제
	}

	// 리스트 초기화 후 다시 채우기
	m_motorListCtrl.SetRedraw(FALSE);
	m_motorListCtrl.DeleteAllItems();

	DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);

	m_motorListCtrl.SetRedraw(TRUE);
	m_motorListCtrl.RedrawWindow();

	// 다시 그리기
	if (m_pParentView)
		m_pParentView->Invalidate();
}

void MotorUI::DeleteMotorRecursive(Motor* motor)
{
	// 자식 목록을 복사하여 안전하게 순회
	std::vector<Motor*> childrenCopy = motor->children;
	for (Motor* subMotor : childrenCopy)
	{
		DeleteMotorRecursive(subMotor);
	}

	motor->children.clear();  // 자식 리스트 비우기

	// 부모에서 이 모터 제거
	if (motor->parentMotor)
	{
		auto& siblings = motor->parentMotor->children;
		auto it = std::find(siblings.begin(), siblings.end(), motor);
		if (it != siblings.end())
			siblings.erase(it);
	}
	else
	{
		auto& roots = m_motorManager.rootMotors;
		auto it = std::find(roots.begin(), roots.end(), motor);
		if (it != roots.end())
			roots.erase(it);
	}

	delete motor;
}

// 모터 저장 기능
void MotorUI::OnSaveMotor()
{
	m_motorManager.SaveMotorData();
}

void MotorUI::OnLoadMotor()
{
	m_motorManager.LoadMotorData();
	// 리스트 컨트롤 초기화
	m_motorListCtrl.DeleteAllItems();

	// 모터 리스트에 있는 모든 모터를 리스트 컨트롤에 추가
	for (auto motor : m_motorManager.rootMotors)
	{
		int index = m_motorListCtrl.GetItemCount();
		CString idStr, typeStr, strPosStr, endPosStr, motorPosStr;
		idStr.Format(_T("%d"), motor->m_id);
		typeStr = motor->isX ? _T("X") : _T("Y");
		strPosStr.Format(_T("(%d, %d)"), motor->strPos.x, motor->strPos.y);
		endPosStr.Format(_T("(%d, %d)"), motor->endPos.x, motor->endPos.y);
		motorPosStr.Format(_T("(%d, %d)"), motor->motorPos.x, motor->motorPos.y);
		m_motorListCtrl.InsertItem(index, idStr);
		m_motorListCtrl.SetItemText(index, 1, typeStr);
		m_motorListCtrl.SetItemText(index, 2, strPosStr);
		m_motorListCtrl.SetItemText(index, 3, endPosStr);
		m_motorListCtrl.SetItemText(index, 4, motorPosStr);
	}

	if (m_pParentView)
		m_pParentView->Invalidate();
}

void MotorUI::OnBnClickedRadioXAxis()
{
	m_endYEdit.EnableWindow(FALSE);
	m_endXEdit.EnableWindow(TRUE);
	CString startY;
	m_startYEdit.GetWindowTextW(startY);
	m_endYEdit.SetWindowTextW(startY);
}

void MotorUI::OnBnClickedRadioYAxis()
{
	m_endXEdit.EnableWindow(FALSE);
	m_endYEdit.EnableWindow(TRUE);
	CString startX;
	m_startXEdit.GetWindowTextW(startX);
	m_endXEdit.SetWindowTextW(startX);
}

void MotorUI::OnChangeStartX()
{
	if (m_radioYAxis.GetSafeHwnd() && m_radioYAxis.GetCheck() == BST_CHECKED)  // Y축 라디오 버튼 선택되었을 때
	{
		CString startX;
		m_startXEdit.GetWindowTextW(startX);
		m_endXEdit.SetWindowTextW(startX);
	}
}

void MotorUI::OnChangeStartY()
{
	if (m_radioXAxis.GetSafeHwnd() && m_radioXAxis.GetCheck() == BST_CHECKED)  // X축 라디오 버튼 선택되었을 때
	{
		CString startY;
		m_startYEdit.GetWindowTextW(startY);
		m_endYEdit.SetWindowTextW(startY);
	}
}

void MotorUI::OnLvnItemChangedMotorList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 선택된 항목이 바뀌었는지 확인
	if ((pListView->uChanged & LVIF_STATE) &&
		(pListView->uNewState & LVIS_SELECTED))
	{
		int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (selectedIndex != -1) {
			CString motorID = m_motorListCtrl.GetItemText(selectedIndex, 0);
			int motorIdInt = _ttoi(motorID);
			Motor* selectedMotor = GetSelectedMotor(motorIdInt);

			if (selectedMotor) {
				CPoint topLeft = selectedMotor->motorPos - selectedMotor->motorSize;
				CPoint bottomRight = selectedMotor->motorPos + selectedMotor->motorSize;
				m_selectedMotorRect.SetRect(topLeft, bottomRight);
			}
		}
	}

	*pResult = 0;
}
