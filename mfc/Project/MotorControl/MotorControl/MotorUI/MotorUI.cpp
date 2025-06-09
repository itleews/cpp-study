#include "pch.h"
#include "MotorUI.h"
#include "../ChildView.h"

BEGIN_MESSAGE_MAP(MotorUI, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(1001, &MotorUI::OnAddMotor) // 추가 버튼 클릭
	ON_BN_CLICKED(1002, &MotorUI::OnRemoveMotor) // 삭제 버튼 클릭
	ON_BN_CLICKED(1003, &MotorUI::OnSaveMotor) // 모터 저장 버튼 클릭
	ON_BN_CLICKED(1004, &MotorUI::OnLoadMotor) // 모터 불러오기 버튼 클릭
	ON_BN_CLICKED(1005, &MotorUI::OnAddRotatingMotor) // 회전 모터 추가 버튼 클릭
	ON_BN_CLICKED(1006, &MotorUI::OnAddSubMotor) // 하위 모터 추가 버튼 클릭
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
	KillTimer(2);  // 타이머 ID 2 제거
	CWnd::OnDestroy();
}

void MotorUI::OnAddMotor()
{
	if (!m_isAddMotorMode && !m_isAddSubmotorMode && !m_isAddRotatingMotorMode) {
		m_isAddMotorMode = true;
		m_groupInput.SetWindowTextW(_T("신규 모터 추가"));
		m_addMotorBtn.SetWindowText(_T("완료"));
		m_removeMotorBtn.SetWindowText(_T("취소"));
		m_addSubMotorBtn.EnableWindow(FALSE);
		UpdatePreviewData();
		return;
	}

	// 하위 모터 조건 검사
	if (m_isAddSubmotorMode && m_previewMotor.parentMotor == nullptr) {
		return;
	}

	m_isAddMotorMode = false; // 모터 추가 모드 해제
	m_isAddSubmotorMode = false; // 하위 모터 추가 모드 해제

	if (m_isAddRotatingMotorMode) {
		m_isAddRotatingMotorMode = false;
		m_motorManager.AddRotatingMotor(m_previewMotor.parentMotor, m_previewMotor.motorPos, m_previewMotor.motorSize, m_previewMotor.rotationSpeed);
	}
	else if (m_previewMotor.parentMotor && m_previewMotor.parentMotor->axis == T) {
		m_motorManager.AddMotor(
			m_previewMotor.parentMotor, // 부모 모터 (없으면 nullptr)
			m_previewMotor.axis,
			m_previewMotor.strPos,
			m_previewMotor.endPos,
			m_previewMotor.motorPos,
			m_previewMotor.motorSize,
			m_previewMotor.motorSpeed,
			true,
			0.0,
			m_previewMotor.parentMotor->rotationSpeed
		);
	}
	else if (m_previewMotor.parentMotor && m_previewMotor.parentMotor->axis != T && m_previewMotor.parentMotor->isRotating) {
		m_motorManager.AddMotor(
			m_previewMotor.parentMotor, // 부모 모터 (없으면 nullptr)
			m_previewMotor.axis,
			m_previewMotor.strPos,
			m_previewMotor.endPos,
			m_previewMotor.motorPos,
			m_previewMotor.motorSize,
			m_previewMotor.motorSpeed,
			true,
			m_previewMotor.rotationAngle,
			m_previewMotor.parentMotor->rotationSpeed
		);
	}
	else {
		m_motorManager.AddMotor(
			m_previewMotor.parentMotor, // 부모 모터 (없으면 nullptr)
			m_previewMotor.axis,
			m_previewMotor.strPos,
			m_previewMotor.endPos,
			m_previewMotor.motorPos,
			m_previewMotor.motorSize,
			m_previewMotor.motorSpeed // 속도
		);
	}

	ResetMotorUI(); // UI 초기화
	// 리스트 컨트롤에 모터 트리 표시
	m_motorListPanel.DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);
}

// 하위 모터 추가
void MotorUI::OnAddSubMotor() {
	// 하위 모터 추가 모드
	if (m_motorManager.rootMotors.empty()) {
		m_labelWarning.SetWindowTextW(_T("상위 모터 없음!"));
		return;
	}
	m_isAddSubmotorMode = true;
	m_selectedMotor = nullptr;
	UpdatePreviewData();
	m_previewMotor.parentMotor = nullptr;
	m_previewMotor.strPos = CPoint(0, 0);
	m_previewMotor.endPos = CPoint(0, 0);
	m_previewMotor.motorPos = CPoint(0, 0);
	m_previewMotor.motorSize = CSize(0, 0);
	m_previewMotor.motorSpeed = 0; // 속도 설정
	m_groupInput.SetWindowText(_T("하위 모터 추가"));
	CString startX;
	m_startXEdit.GetWindowTextW(startX);
	m_endXEdit.SetWindowTextW(startX);
	CString startY;
	m_startYEdit.GetWindowTextW(startY);
	m_endYEdit.SetWindowTextW(startY);
	m_width.SetWindowTextW(_T("10"));
	m_height.SetWindowTextW(_T("10"));
	m_addMotorBtn.SetWindowText(_T("완료"));
	m_removeMotorBtn.SetWindowText(_T("취소"));
	m_addSubMotorBtn.EnableWindow(FALSE);
}

void MotorUI::OnAddRotatingMotor() {
	m_isAddRotatingMotorMode = true;
	m_groupInput.SetWindowText(_T("회전 모터 추가"));
	m_addMotorBtn.SetWindowText(_T("완료"));
	m_removeMotorBtn.SetWindowText(_T("취소"));
	m_addRotMotorBtn.EnableWindow(FALSE);
	m_addSubMotorBtn.EnableWindow(FALSE);
	m_endXEdit.SetWindowTextW(_T("10"));
	m_endYEdit.EnableWindow(FALSE);
	m_endYEdit.SetWindowTextW(_T(""));
	m_labelSize.SetWindowTextW(_T("반지름(r)"));
	m_height.EnableWindow(FALSE);
	m_height.SetWindowTextW(_T(""));
	m_radioYAxis.EnableWindow(FALSE);
	m_labelStart.SetWindowTextW(_T("모터 위치(x, y)"));
	m_labelEnd.SetWindowTextW(_T("회전속도(˚/s)"));
	m_radioXAxis.SetWindowTextW(_T("T축"));
	m_radioXAxis.ShowWindow(SW_HIDE);
	m_speed.SetWindowTextW(_T(""));
	m_speed.EnableWindow(FALSE);
	m_labelSpeed.SetWindowTextW(_T(""));
	UpdatePreviewData();
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

void MotorUI::UpdateMotorListTexts() {
	std::vector<DisplayRow> rows;
	for (Motor* root : m_motorManager.rootMotors)
		m_motorListPanel.BuildDisplayRows(root, 0, CPoint(0, 0), rows);

	m_motorListCtrl.SetRedraw(FALSE);

	int itemCount = m_motorListCtrl.GetItemCount();
	for (int i = 0; i < itemCount && i < (int)rows.size(); ++i) {
		m_motorListCtrl.SetItemText(i, 2, rows[i].strPos);
		m_motorListCtrl.SetItemText(i, 3, rows[i].endPos);
		m_motorListCtrl.SetItemText(i, 4, rows[i].relMotorPos);
		m_motorListCtrl.SetItemText(i, 5, rows[i].absMotorPos);
	}

	m_motorListCtrl.SetRedraw(TRUE);
	m_motorListCtrl.UpdateWindow();
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
	if (m_isAddMotorMode || m_isAddSubmotorMode || m_isAddRotatingMotorMode) {
		m_isAddMotorMode = false; // 모터 추가 모드 해제
		m_isAddSubmotorMode = false; // 하위 모터 추가 모드 해제
		m_isAddRotatingMotorMode = false; // 회전 모터 추가 모드 해제
		ResetMotorUI(); // UI 초기화
		return;
	}

	std::vector<int> selectedMotorIds;
	int index = -1;
	while ((index = m_motorListCtrl.GetNextItem(index, LVNI_SELECTED)) != -1)
	{
		CString motorIDStr = m_motorListCtrl.GetItemText(index, 0);
		int motorIdInt = _ttoi(motorIDStr);
		selectedMotorIds.push_back(motorIdInt);
	}

	if (selectedMotorIds.empty())
	{
		m_labelWarning.SetWindowTextW(_T("삭제할 모터 선택!"));
		return;
	}

	m_motorManager.RemoveMotors(selectedMotorIds);

	// 삭제 후 UI 갱신
	m_motorListPanel.DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);
	ResetMotorUI();
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

CString MotorUI::FilterNumericInput(const CString& input)
{
	CString result;
	bool dotUsed = false;
	int decimalCount = 0;

	for (int i = 0; i < input.GetLength(); ++i) {
		TCHAR ch = input[i];
		if (_istdigit(ch)) {
			if (dotUsed) {
				if (decimalCount < 2) {
					result += ch;
					decimalCount++;
				}
			}
			else {
				result += ch;
			}
		}
		else if (ch == '-' && i == 0) {
			result += ch;
		}
		else if (ch == '.' && !dotUsed) {
			result += ch;
			dotUsed = true;
		}
		// 그 외 문자 무시
	}

	// double로 변환해서 비교
	double val = _ttof(result);
	if (val > 1000.0) {
		result = _T("1000");
	}
	else if (val < -1000.0) {
		result = _T("-1000");
	}

	return result;
}


void MotorUI::OnEditChanged()
{
	for (auto edit : m_numericEdits) {
		if (!::IsWindow(edit->GetSafeHwnd()))  // 윈도우가 유효한지 체크
			return;  // 생성 안 됐으면 함수 종료

		CString text;
		edit->GetWindowText(text);
		CString filtered = FilterNumericInput(text);
		if (text != filtered) {
			edit->SetWindowText(filtered);
			edit->SetSel(filtered.GetLength(), filtered.GetLength());
		}
	}
	UpdatePreviewData();
}

void MotorUI::UpdatePreviewData()
{
	if (!m_isAddMotorMode && !m_isAddSubmotorMode && !m_isAddRotatingMotorMode) return;

	MotorPreviewInputData input;
	input.isAddMotorMode = m_isAddMotorMode;
	input.isAddSubmotorMode = m_isAddSubmotorMode;
	input.isAddRotatingMotorMode = m_isAddRotatingMotorMode;

	m_startXEdit.GetWindowTextW(input.sx);
	m_startYEdit.GetWindowTextW(input.sy);
	m_endXEdit.GetWindowTextW(input.ex);
	m_endYEdit.GetWindowTextW(input.ey);
	m_width.GetWindowTextW(input.width);
	m_height.GetWindowTextW(input.height);
	m_speed.GetWindowTextW(input.speed);

	input.axis = (m_radioXAxis.GetCheck() == BST_CHECKED) ? X : Y;

	// 하위 모터 모드인 경우에만 parentMotor 설정
	input.parentMotor = nullptr;
	if (m_isAddSubmotorMode) {
		int id = GetSelectedMotorId();
		if (id != -1) {
			input.parentMotor = GetSelectedMotor(id);
		}
	}

	m_previewMotor = m_motorPreviewPanel.UpdatePreview(input);

	if (!m_previewMotor.isValid) {
		m_addMotorBtn.EnableWindow(false);
		m_labelWarning.SetWindowTextW(_T("올바르지 않은 입력"));
	}
	else {
		m_addMotorBtn.EnableWindow(true);
		m_labelWarning.SetWindowTextW(_T(""));
	}
}

int MotorUI::GetSelectedMotorId() const
{
	int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex == -1) return -1;

	CString motorID = m_motorListCtrl.GetItemText(selectedIndex, 0);
	return _ttoi(motorID);
}

// 모터 저장 기능
void MotorUI::OnSaveMotor()
{
	m_isSaveMotorMode = true;
	m_motorManager.SaveMotorData();
	m_isSaveMotorMode = false;
}

void MotorUI::OnLoadMotor()
{
	m_motorManager.LoadMotorData();
	// 리스트 컨트롤 초기화
	m_motorListCtrl.DeleteAllItems();

	// 모터 리스트에 있는 모든 모터를 리스트 컨트롤에 추가
	m_motorListPanel.DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);

	if (m_pParentView)
		m_pParentView->Invalidate();
}

void MotorUI::MoveSelectedAxis(int dx, int dy)
{
	int motorIdInt = GetSelectedMotorId();
	Motor* selectedMotor = GetSelectedMotor(motorIdInt);
	if (selectedMotor)
	{
		m_motorMovePanel.MoveMotorRecursive(selectedMotor, dx, dy);
	}
}

void MotorUI::ResetMotorUI()
{
	m_groupInput.SetWindowText(_T("모터 관리"));
	m_addMotorBtn.SetWindowText(_T("추가"));
	m_removeMotorBtn.SetWindowText(_T("삭제"));
	m_addMotorBtn.EnableWindow(TRUE);
	m_addRotMotorBtn.EnableWindow(TRUE);
	m_addSubMotorBtn.EnableWindow(TRUE);
	m_endYEdit.EnableWindow(FALSE);
	m_height.EnableWindow(TRUE);
	m_speed.EnableWindow(TRUE);
	// 라벨 텍스트 초기화
	m_labelStart.SetWindowTextW(_T("시작 위치(x, y)"));
	m_labelEnd.SetWindowTextW(_T("끝 위치(x, y)"));
	m_labelSize.SetWindowTextW(_T("크기(W, H)"));
	m_labelAxis.SetWindowTextW(_T("기준 축"));
	m_labelSpeed.SetWindowTextW(_T("모터 속도"));
	m_labelWarning.SetWindowTextW(_T("\"추가\"로 시작"));
	// 입력 필드 초기화
	m_startXEdit.SetWindowTextW(_T("0"));
	m_startYEdit.SetWindowTextW(_T("100"));
	m_endXEdit.SetWindowTextW(_T("1000"));
	m_endYEdit.SetWindowTextW(_T("100"));
	m_width.SetWindowTextW(_T("100"));
	m_height.SetWindowTextW(_T("100"));
	m_speed.SetWindowTextW(_T("100"));
	// 라디오 버튼 초기화
	m_radioXAxis.ShowWindow(SW_SHOW);
	m_radioXAxis.SetWindowTextW(_T("X축"));
	m_radioXAxis.SetCheck(TRUE);
	m_radioYAxis.SetCheck(FALSE);
	m_radioYAxis.EnableWindow(TRUE);
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
	OnEditChanged();  // 시작 위치 변경 시 미리보기 업데이트

	if (m_isAddRotatingMotorMode)
		return;

	if (m_radioYAxis.GetSafeHwnd() && m_radioYAxis.GetCheck() == BST_CHECKED)  // Y축 라디오 버튼 선택되었을 때
	{
		CString startX;
		m_startXEdit.GetWindowTextW(startX);
		m_endXEdit.SetWindowTextW(startX);
	}
}

void MotorUI::OnChangeStartY()
{
	OnEditChanged();

	if (m_isAddRotatingMotorMode)
		return;

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
			m_selectedMotor = selectedMotor;
		}
	}

	if (m_isAddSubmotorMode) {
		UpdatePreviewData();
	}

	*pResult = 0;
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
	m_numericEdits = {&m_startXEdit, &m_startYEdit, &m_endXEdit, &m_endYEdit, &m_width, &m_height, &m_speed};
	CreateEdit(m_startXEdit, 2001, _T("0"));
	CreateEdit(m_startYEdit, 2002, _T("100"));
	CreateEdit(m_endXEdit, 2003, _T("1000"));
	CreateEdit(m_endYEdit, 2004, _T("100"));
	CreateEdit(m_width, 2005, _T("100"));
	CreateEdit(m_height, 2006, _T("100"));
	CreateEdit(m_speed, 2008, _T("100"));
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
	m_labelWarning.Create(_T("\"추가\"로 시작"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 0, 0), this);
}

HBRUSH MotorUI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetSafeHwnd() == m_labelWarning.GetSafeHwnd())
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(OPAQUE); // 투명 대신 불투명으로 바꿔서 배경 채우기
		static CBrush brushBackground(RGB(255, 255, 255)); // 배경색(흰색 등)으로 채우기
		return (HBRUSH)brushBackground.GetSafeHandle();
	}
	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

void MotorUI::CreateMotorButtons()
{
	CreateButton(m_addMotorBtn, 1001, _T("추가"));
	CreateButton(m_removeMotorBtn, 1002, _T("삭제"));
	CreateButton(m_saveMotorBtn, 1003, _T("저장"));
	CreateButton(m_loadMotorBtn, 1004, _T("불러오기"));
	CreateButton(m_addRotMotorBtn, 1005, _T("회전 모터 추가"));
	CreateButton(m_addSubMotorBtn, 1006, _T("하위 모터 추가"));
}

void MotorUI::CreateControlButtons()
{
	const TCHAR* labels[] = { _T("▲"), _T("▼"), _T("◀"), _T("▶") };
	const int ids[] = { 4001, 4002, 4003, 4004 };

	for (int i = 0; i < 4; ++i)
	{
		m_controlBtn[i].Create(labels[i], WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ids[i]);
	}
}

void MotorUI::CreateEdit(CEdit& edit, int id, const CString& defaultText)
{
	edit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this, id);
	edit.SetWindowTextW(defaultText);

	// 숫자 필터 대상이면 vector에 추가
	m_numericEdits.push_back(&edit);
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
	m_addMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row1Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row2Y = row1Y + fieldHeight + rowGap;
	m_labelEnd.SetWindowPos(nullptr, inputStartX + groupPadding, row2Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_endXEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_endYEdit.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row2Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_removeMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row2Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row3Y = row2Y + fieldHeight + rowGap;
	m_labelSize.SetWindowPos(nullptr, inputStartX + groupPadding, row3Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_width.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_height.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + fieldWidth + colGap, row3Y, fieldWidth, fieldHeight, SWP_NOZORDER);
	m_labelWarning.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row3Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row4Y = row3Y + fieldHeight + rowGap;
	m_labelAxis.SetWindowPos(nullptr, inputStartX + groupPadding, row4Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_radioXAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row4Y, 60, fieldHeight, SWP_NOZORDER);
	m_radioYAxis.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + 70, row4Y, 60, fieldHeight, SWP_NOZORDER);
	m_addRotMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row4Y, labelWidth, btnHeight, SWP_NOZORDER);

	int row5Y = row4Y + fieldHeight + rowGap;
	m_labelSpeed.SetWindowPos(nullptr, inputStartX + groupPadding, row5Y, labelWidth, fieldHeight, SWP_NOZORDER);
	m_speed.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap, row5Y, fieldWidth * 2 + colGap, fieldHeight, SWP_NOZORDER);
	m_addSubMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + labelWidth + colGap, row5Y, labelWidth, btnHeight, SWP_NOZORDER);

	int inputGroupHeight = (row5Y + fieldHeight + groupPadding) - inputStartY;
	m_groupInput.SetWindowPos(nullptr, rightX, inputStartY, rightWidth, inputGroupHeight, SWP_NOZORDER);

	// 3. 버튼 그룹박스
	int buttonGroupY = inputStartY + inputGroupHeight + sectionGap;

	int buttonY = buttonGroupY + btnHeight + rowGap;
	m_saveMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY, halfWidth, btnHeight, SWP_NOZORDER);
	m_loadMotorBtn.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY, halfWidth, btnHeight, SWP_NOZORDER);

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
	m_controlBtn[0].SetWindowPos(nullptr, centerX - bw / 2, centerY - bh - rowGap, bw, bh, SWP_NOZORDER);

	// 아래 (1) 버튼 위치
	m_controlBtn[1].SetWindowPos(nullptr, centerX - bw / 2, centerY + bh + rowGap, bw, bh, SWP_NOZORDER);

	// 왼 (2) 버튼 위치
	m_controlBtn[2].SetWindowPos(nullptr, centerX - bw * 2 + colGap + 50, centerY, bw, bh, SWP_NOZORDER);

	// 오 (3) 버튼 위치
	m_controlBtn[3].SetWindowPos(nullptr, centerX + bw / 2 + colGap, centerY, bw, bh, SWP_NOZORDER);

	// 그룹 전체의 크기 설정
	m_groupControl.SetWindowPos(nullptr, rightX, controlTop, rightWidth, controlHeight, SWP_NOZORDER);

}