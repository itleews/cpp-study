#include "pch.h"
#include "MotorUI.h"
#include "ChildView.h"
#include <algorithm>

BEGIN_MESSAGE_MAP(MotorUI, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(1001, &MotorUI::OnAddMotor) // 축 추가 버튼 클릭
	ON_BN_CLICKED(2007, &MotorUI::OnAddSubMotor) // 하위 모터 추가 버튼 클릭
	ON_BN_CLICKED(1002, &MotorUI::OnRemoveMotor) // 축 삭제 버튼 클릭
	ON_BN_CLICKED(1003, &MotorUI::OnSaveMotor) // 모터 저장 버튼 클릭
	ON_BN_CLICKED(1004, &MotorUI::OnLoadMotor) // 모터 불러오기 버튼 클릭
	ON_EN_CHANGE(2001, &MotorUI::OnChangeStartX)  // m_startXEdit
	ON_EN_CHANGE(2002, &MotorUI::OnChangeStartY)  // m_startYEdit
	ON_EN_CHANGE(2003, &MotorUI::OnEditChanged)
	ON_EN_CHANGE(2004, &MotorUI::OnEditChanged)
	ON_EN_CHANGE(2005, &MotorUI::OnEditChanged)
	ON_EN_CHANGE(2006, &MotorUI::OnEditChanged)
	ON_EN_CHANGE(2008, &MotorUI::OnEditChanged)
	ON_BN_CLICKED(3003, &MotorUI::OnBnClickedRadioXAxis) // X축 라디오 버튼 클릭
	ON_BN_CLICKED(3004, &MotorUI::OnBnClickedRadioYAxis) // Y축 라디오 버튼 클릭
	ON_NOTIFY(NM_CLICK, 1, &MotorUI::OnNMClickMotorList) // 리스트 컨트롤 항목 변경
	ON_BN_CLICKED(4001, &MotorUI::OnBnClickedControlUpButton) // 조작부 버튼 클릭
	ON_BN_CLICKED(4002, &MotorUI::OnBnClickedControlDownButton) // 조작부 버튼 클릭
	ON_BN_CLICKED(4003, &MotorUI::OnBnClickedControlLeftButton) // 조작부 버튼 클릭
	ON_BN_CLICKED(4004, &MotorUI::OnBnClickedControlRightButton) // 조작부 버튼 클릭
END_MESSAGE_MAP()

const int MOVE_DELTA = 5;  // 한 번에 움직이는 거리(픽셀 단위)

int MotorUI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateListControl();
	CreateGroupBoxes();
	CreateInputFields();
	CreateRadioButtons();
	CreateLabels();
	CreateMotorButtons();
	CreateControlButtons();

	SetTimer(2, 16, NULL);
	return 0;
}

void MotorUI::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2)
	{
		if (!m_isAddSubmotorMode)
			UpdateMotorListTexts();
	}

	CWnd::OnTimer(nIDEvent);
}

void MotorUI::OnDestroy()
{
	KillTimer(2);  // 타이머 ID 1 제거
	CWnd::OnDestroy();
}


void MotorUI::BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows)
{
	if (!node) return;

	CString indent;
	for (int i = 0; i < depth; ++i)
		indent += _T("    ");

	CString idStr;
	idStr.Format(_T("%d"), node->m_id);

	CString typeStr = node->isX ? _T("X") : _T("Y");

	CPoint relStrPos = node->strPos - parentOrigin;
	CPoint relEndPos = node->endPos - parentOrigin;
	CPoint relMotorPos = node->motorPos - parentOrigin;

	CString strPosStr, endPosStr, relMotorPosStr, absMotorPosStr;
	strPosStr.Format(_T("(%d, %d)"), relStrPos.x, relStrPos.y);
	endPosStr.Format(_T("(%d, %d)"), relEndPos.x, relEndPos.y);
	relMotorPosStr.Format(_T("(%d, %d)"), relMotorPos.x, relMotorPos.y);
	absMotorPosStr.Format(_T("(%d, %d)"), node->motorPos.x, node->motorPos.y);

	DisplayRow row = {
		indent + idStr,
		typeStr,
		strPosStr,
		endPosStr,
		relMotorPosStr,
		absMotorPosStr
	};
	outRows.push_back(row);

	for (Motor* child : node->children)
	{
		CPoint newOrigin = node->motorPos - node->motorSize;
		BuildDisplayRows(child, depth + 1, newOrigin, outRows);
	}
}

void MotorUI::DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors)
{
	listCtrl.DeleteAllItems();

	std::vector<DisplayRow> rows;
	for (Motor* root : rootMotors)
		BuildDisplayRows(root, 0, CPoint(0, 0), rows);

	for (int i = 0; i < (int)rows.size(); ++i)
	{
		listCtrl.InsertItem(i, rows[i].id);
		listCtrl.SetItemText(i, 1, rows[i].type);
		listCtrl.SetItemText(i, 2, rows[i].strPos);
		listCtrl.SetItemText(i, 3, rows[i].endPos);
		listCtrl.SetItemText(i, 4, rows[i].relMotorPos);
		listCtrl.SetItemText(i, 5, rows[i].absMotorPos);
	}
}

void MotorUI::UpdateMotorListTexts()
{
	std::vector<DisplayRow> rows;
	for (Motor* root : m_motorManager.rootMotors)
		BuildDisplayRows(root, 0, CPoint(0, 0), rows);

	int itemCount = m_motorListCtrl.GetItemCount();

	m_motorListCtrl.SetRedraw(FALSE);

	for (int i = 0; i < itemCount && i < (int)rows.size(); ++i)
	{
		m_motorListCtrl.SetItemText(i, 2, rows[i].strPos);
		m_motorListCtrl.SetItemText(i, 3, rows[i].endPos);
		m_motorListCtrl.SetItemText(i, 4, rows[i].relMotorPos);
		m_motorListCtrl.SetItemText(i, 5, rows[i].absMotorPos);
	}

	m_motorListCtrl.SetRedraw(TRUE);
	m_motorListCtrl.UpdateWindow();
}

void MotorUI::OnAddMotor()
{
	if (!m_isAddMotorMode && !m_isAddSubmotorMode) {
		m_isAddMotorMode = true;
		m_addMotorButton.SetWindowText(_T("완료"));
		m_removeMotorButton.SetWindowText(_T("취소"));
		UpdatePreview();
		return;
	}

	// 하위 모터 조건 검사
	if (m_isAddSubmotorMode && m_previewMotor.parentMotor == nullptr) {
		AfxMessageBox(_T("하위 모터를 추가할 상위 모터를 선택하세요!"));
		return;
	}

	m_isAddMotorMode = false;
	m_isAddSubmotorMode = false;
	m_addMotorButton.SetWindowText(_T("추가"));
	m_removeMotorButton.SetWindowText(_T("삭제"));

	m_motorManager.AddMotor(
		m_previewMotor.parentMotor, // 부모 모터 (없으면 nullptr)
		m_previewMotor.isX,
		m_previewMotor.strPos,
		m_previewMotor.endPos,
		m_previewMotor.motorPos,
		m_previewMotor.motorSize,
		m_previewMotor.motorSpeed // 속도
	);

	// 리스트 컨트롤에 모터 트리 표시
	DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);
}


// 하위 모터 추가
void MotorUI::OnAddSubMotor() {
	// 하위 모터 추가 모드
	if (m_motorManager.rootMotors.empty()) {
		AfxMessageBox(_T("하위 모터를 추가할 상위 모터가 없습니다."));
		return;
	}
	m_isAddSubmotorMode = true;
	m_selectedMotorRect.SetRect(0, 0, 0, 0);

	UpdatePreview();
	m_previewMotor.parentMotor = nullptr;
	m_previewMotor.strPos = CPoint(0, 0);
	m_previewMotor.endPos = CPoint(0, 0);
	m_previewMotor.motorPos = CPoint(0, 0);
	m_previewMotor.motorSize = CSize(0, 0);
	m_previewMotor.motorSpeed = 0; // 속도 설정
	m_groupInput.SetWindowText(_T("하위 모터 추가"));
	m_addMotorButton.SetWindowText(_T("완료"));
	m_removeMotorButton.SetWindowText(_T("취소"));
	m_addSubMotorButton.EnableWindow(FALSE);
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

	if (m_isAddMotorMode) {
		m_isAddMotorMode = false; // 모터 추가 모드 해제
		m_addMotorButton.SetWindowText(_T("추가"));
		m_removeMotorButton.SetWindowText(_T("삭제"));
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

	DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);
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

void MotorUI::OnEditChanged()
{
	UpdatePreview();
}

void MotorUI::UpdatePreview()
{
	if (!m_isAddMotorMode && !m_isAddSubmotorMode) return;

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

			m_groupInput.SetWindowText(_T("모터 관리"));
			m_addSubMotorButton.EnableWindow(TRUE);
		}
		else {
			return;
		}
	}

	int motorX = m_radioXAxis.GetCheck() == BST_CHECKED ? (end.x - start.x) / 2 + start.x : end.x;
	int motorY = m_radioXAxis.GetCheck() == BST_CHECKED ? start.y : (end.y - start.y) / 2 + start.y;

	m_previewMotor.parentMotor = parentMotor;
	m_previewMotor.isX = m_radioXAxis.GetCheck() == BST_CHECKED;
	m_previewMotor.strPos = start;
	m_previewMotor.endPos = end;
	m_previewMotor.motorPos = CPoint(motorX, motorY);
	m_previewMotor.motorSize = CSize(motor.cx / 2, motor.cy / 2);
	m_previewMotor.motorSpeed = speed; // 속도 설정
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

void MotorUI::OnNMClickMotorList(NMHDR* pNMHDR, LRESULT* pResult)
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

	*pResult = 0;
}


void MotorUI::MoveMotorRecursive(Motor* motor, int dx, int dy)
{
	if (!motor) return;

	if (motor->parentMotor) {
		CPoint parentTopLeft = motor->parentMotor->motorPos - motor->parentMotor->motorSize;
		CPoint parentBottomRight = motor->parentMotor->motorPos + motor->parentMotor->motorSize;

		CPoint newStrPos = motor->strPos + CPoint(dx, dy);
		CPoint newEndPos = motor->endPos + CPoint(dx, dy);

		if (newStrPos.x < parentTopLeft.x || newStrPos.y < parentTopLeft.y ||
			newEndPos.x > parentBottomRight.x || newEndPos.y > parentBottomRight.y)
		{
			return; // 부모 영역을 벗어나므로 이동하지 않음
		}
	}

	motor->strPos.x += dx;
	motor->strPos.y += dy;
	motor->endPos.x += dx;
	motor->endPos.y += dy;
	motor->motorPos.x += dx;
	motor->motorPos.y += dy;

	for (Motor* child : motor->children)
	{
		MoveMotorRecursive(child, dx, dy);
	}
}

void MotorUI::MoveSelectedAxis(int dx, int dy)
{
	int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex == -1) return;

	CString motorID = m_motorListCtrl.GetItemText(selectedIndex, 0);
	int motorIdInt = _ttoi(motorID);

	Motor* selectedMotor = GetSelectedMotor(motorIdInt);
	if (selectedMotor)
	{
		MoveMotorRecursive(selectedMotor, dx, dy);
	}
}

void MotorUI::OnBnClickedControlUpButton()
{
	MoveSelectedAxis(0, -MOVE_DELTA);
}

void MotorUI::OnBnClickedControlDownButton()
{
	MoveSelectedAxis(0, MOVE_DELTA);
}

void MotorUI::OnBnClickedControlLeftButton()
{
	MoveSelectedAxis(-MOVE_DELTA, 0);
}

void MotorUI::OnBnClickedControlRightButton()
{
	MoveSelectedAxis(MOVE_DELTA, 0);
}

void MotorUI::CreateListControl()
{
	m_motorListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 0, 0), this, 1);
	m_motorListCtrl.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_motorListCtrl.InsertColumn(0, _T("모터 ID"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(1, _T("축"), LVCFMT_LEFT, 50);
	m_motorListCtrl.InsertColumn(2, _T("시작 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(3, _T("끝 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(4, _T("현재 위치"), LVCFMT_LEFT, 100);
	m_motorListCtrl.InsertColumn(5, _T("절대 위치"), LVCFMT_LEFT, 100);
}

void MotorUI::CreateGroupBoxes()
{
	m_groupInput.Create(_T("모터 관리"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3000);
	m_groupButtons.Create(_T("파일 관리"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3001);
	m_groupControl.Create(_T("모터 조작부"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3002);
}


void MotorUI::CreateInputFields()
{
	CreateEdit(m_startXEdit, 2001, _T("0"));
	CreateEdit(m_startYEdit, 2002, _T("100"));
	CreateEdit(m_endXEdit, 2003, _T("1000"));
	CreateEdit(m_endYEdit, 2004, _T("100"));
	CreateEdit(m_width, 2005, _T("100"));
	CreateEdit(m_height, 2006, _T("100"));
	CreateEdit(m_speed, 2008, _T("100"));

	m_addSubMotorButton.Create(_T("하위 모터 추가"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 2007);
}

void MotorUI::CreateRadioButtons()
{
	m_radioXAxis.Create(_T("X축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3003);
	m_radioYAxis.Create(_T("Y축"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3004);

	m_radioXAxis.SetCheck(TRUE);        // 기본 X축
	m_endYEdit.EnableWindow(FALSE);     // Y좌표 비활성화
}

void MotorUI::CreateLabels()
{
	CreateStatic(m_labelStart, _T("시작 위치(x, y)"));
	CreateStatic(m_labelEnd, _T("끝 위치(x, y)"));
	CreateStatic(m_labelSize, _T("크기(W, H)"));
	CreateStatic(m_labelAxis, _T("기준 축"));
	CreateStatic(m_labelSpeed, _T("모터 속도"));
}


void MotorUI::CreateMotorButtons()
{
	CreateButton(m_addMotorButton, 1001, _T("추가"));
	CreateButton(m_removeMotorButton, 1002, _T("삭제"));
	CreateButton(m_saveMotorButton, 1003, _T("저장"));
	CreateButton(m_loadMotorButton, 1004, _T("불러오기"));
}

void MotorUI::CreateControlButtons()
{
	const TCHAR* labels[] = { _T("▲"), _T("▼"), _T("◀"), _T("▶") };
	const int ids[] = { 4001, 4002, 4003, 4004 };

	for (int i = 0; i < 4; ++i)
	{
		m_controlButton[i].Create(labels[i], WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ids[i]);
	}
}


void MotorUI::CreateEdit(CEdit& edit, int id, const CString& defaultText)
{
	edit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), this, id);
	edit.SetWindowTextW(defaultText);
}

void MotorUI::CreateStatic(CStatic& label, const CString& text)
{
	label.Create(text, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
}

void MotorUI::CreateButton(CButton& button, int id, const CString& text)
{
	button.Create(text, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, id);
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