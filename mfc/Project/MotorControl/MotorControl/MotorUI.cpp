#include "pch.h"
#include "MotorUI.h"
#include "ChildView.h"
#include <algorithm>

BEGIN_MESSAGE_MAP(MotorUI, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(1001, &MotorUI::OnAddMotor) // �� �߰� ��ư Ŭ��
	ON_BN_CLICKED(1002, &MotorUI::OnRemoveMotor) // �� ���� ��ư Ŭ��
	ON_BN_CLICKED(1003, &MotorUI::OnSaveMotor) // ���� ���� ��ư Ŭ��
	ON_BN_CLICKED(1004, &MotorUI::OnLoadMotor) // ���� �ҷ����� ��ư Ŭ��
	ON_EN_CHANGE(2001, &MotorUI::OnChangeStartX)  // m_startXEdit
	ON_EN_CHANGE(2002, &MotorUI::OnChangeStartY)  // m_startYEdit
	ON_BN_CLICKED(3003, &MotorUI::OnBnClickedRadioXAxis) // X�� ���� ��ư Ŭ��
	ON_BN_CLICKED(3004, &MotorUI::OnBnClickedRadioYAxis) // Y�� ���� ��ư Ŭ��
END_MESSAGE_MAP()

int MotorUI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    // ����Ʈ ��Ʈ�� ����
    m_motorListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 0, 0), this, 1);
    m_motorListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER);
    m_motorListCtrl.InsertColumn(0, _T("���� ID"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(1, _T("���� ��"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(2, _T("���� ��ġ"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(3, _T("�� ��ġ"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(4, _T("���� ��ġ"), LVCFMT_LEFT, 100);

    // �׷�ڽ� ����
    m_groupInput.Create(_T("��ġ �� ũ�� ����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3000);
    m_groupButtons.Create(_T("���� ����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3001);
	m_groupControl.Create(_T("���� ���ۺ�"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), this, 3002);

    // �Է� �ʵ� ����
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

	m_subMotorCheck.Create(_T("���� ���ͷ� �߰�"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(0, 0, 0, 0), this, 2007);
	m_subMotorCheck.SetCheck(BST_UNCHECKED);

    // ���� ��ư ����
    m_radioXAxis.Create(_T("X��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3003);
    m_radioYAxis.Create(_T("Y��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), this, 3004);
    m_radioXAxis.SetCheck(TRUE);  // �⺻���� X�� ����
    m_endYEdit.EnableWindow(FALSE);  // Y�� ��Ȱ��ȭ

    // �� ����
    m_labelStart.Create(_T("���� ��ġ(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelEnd.Create(_T("�� ��ġ(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelSize.Create(_T("ũ��(W, H)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    m_labelAxis.Create(_T("���� ��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);

    // ���� ���� ��ư ����
    m_addMotorButton.Create(_T("�߰�"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1001);
    m_removeMotorButton.Create(_T("����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1002);
    m_saveMotorButton.Create(_T("����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1003);
    m_loadMotorButton.Create(_T("�ҷ�����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1004);

	// ���ۺ� ��ư ����
	m_controlButton[0].Create(_T("��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4001);
	m_controlButton[1].Create(_T("��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4002);
	m_controlButton[2].Create(_T("��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4003);
	m_controlButton[3].Create(_T("��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 4004);

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

	// 1. ����Ʈ ��Ʈ��
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

	int halfWidth = (inputGroupWidth - groupPadding * 2 - colGap) / 2 - 10;

	// 2. �Է� �׷�ڽ�
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
	m_subMotorCheck.SetWindowPos(nullptr, inputStartX + groupPadding + labelWidth + colGap + 180, row4Y, 180, fieldHeight, SWP_NOZORDER);

	int inputGroupHeight = (row4Y + fieldHeight + groupPadding) - inputStartY;
	m_groupInput.SetWindowPos(nullptr, rightX, inputStartY, rightWidth, inputGroupHeight, SWP_NOZORDER);

	// 3. ��ư �׷�ڽ�
	int buttonGroupY = inputStartY + inputGroupHeight + sectionGap;

	int buttonY = buttonGroupY + 20;
	m_addMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY, halfWidth, btnHeight, SWP_NOZORDER);
	m_removeMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY, halfWidth, btnHeight, SWP_NOZORDER);

	int buttonY2 = buttonY + btnHeight + rowGap;
	m_saveMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding, buttonY2, halfWidth, btnHeight, SWP_NOZORDER);
	m_loadMotorButton.SetWindowPos(nullptr, inputStartX + groupPadding + halfWidth + colGap, buttonY2, halfWidth, btnHeight, SWP_NOZORDER);

	int buttonGroupHeight = (buttonY2 + btnHeight + groupPadding) - buttonGroupY;
	m_groupButtons.SetWindowPos(nullptr, rightX, buttonGroupY, rightWidth, buttonGroupHeight, SWP_NOZORDER);

	// 4. ���ۺ�
	int controlTop = buttonGroupY + buttonGroupHeight + sectionGap;
	int controlHeight = cy - controlTop - margin;

	int controlButtonWidth = (rightWidth - 3 * colGap) / 4;
	int controlButtonHeight = (controlHeight - 3 * rowGap) / 4;

	// ��ư ��ġ�� �߽���
	int centerX = rightX + rightWidth / 2;
	int centerY = controlTop + controlHeight / 2;

	int bw = controlButtonWidth;
	int bh = controlButtonHeight;

	// �� (0) ��ư ��ġ
	m_controlButton[0].SetWindowPos(nullptr, centerX - bw / 2, centerY - bh - rowGap, bw, bh, SWP_NOZORDER);

	// �Ʒ� (1) ��ư ��ġ
	m_controlButton[1].SetWindowPos(nullptr, centerX - bw / 2, centerY + bh + rowGap, bw, bh, SWP_NOZORDER);

	// �� (2) ��ư ��ġ
	m_controlButton[2].SetWindowPos(nullptr, centerX - bw - colGap, centerY, bw, bh, SWP_NOZORDER);

	// �� (3) ��ư ��ġ
	m_controlButton[3].SetWindowPos(nullptr, centerX + bw + colGap, centerY, bw, bh, SWP_NOZORDER);

	// �׷� ��ü�� ũ�� ����
	m_groupControl.SetWindowPos(nullptr, rightX, controlTop, rightWidth, controlHeight, SWP_NOZORDER);

}

void MotorUI::DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors)
{
	listCtrl.DeleteAllItems(); // ���� ���� ����

	int itemIndex = 0;
	// ��� �ֻ��� ���Ϳ� ���� ó��
	for (Motor* root : rootMotors)
	{
		DisplayMotorRecursive(listCtrl, root, 0, itemIndex);
	}
}

void MotorUI::DisplayMotorRecursive(CListCtrl& listCtrl, Motor* node, int depth, int& itemIndex)
{
	if (!node) return;

	CString displayMotorID;

	// depth��ŭ �鿩���� �߰�
	for (int i = 0; i < depth; ++i)
		displayMotorID += _T("    "); // �����̽� 4��

	CString idStr;
	idStr.Format(_T("%d"), node->m_id);
	displayMotorID += idStr;

	CString typeStr, strPosStr, endPosStr, motorPosStr;
	typeStr = node->isX ? _T("X") : _T("Y");
	strPosStr.Format(_T("(%d, %d)"), node->strPos.x, node->strPos.y);
	endPosStr.Format(_T("(%d, %d)"), node->endPos.x, node->endPos.y);
	motorPosStr.Format(_T("(%d, %d)"), node->motorPos.x, node->motorPos.y);
	listCtrl.InsertItem(itemIndex, displayMotorID);
	listCtrl.SetItemText(itemIndex, 1, typeStr);
	listCtrl.SetItemText(itemIndex, 2, strPosStr);
	listCtrl.SetItemText(itemIndex, 3, endPosStr);
	listCtrl.SetItemText(itemIndex, 4, motorPosStr);

	++itemIndex;

	// �ڽĵ鵵 ��������� �߰�
	for (Motor* child : node->children)
	{
		DisplayMotorRecursive(listCtrl, child, depth + 1, itemIndex);
	}
}

void MotorUI::OnAddMotor()
{
	// ��ġ �ؽ�Ʈ �б�
	CString sx, sy, ex, ey, w, h;
	m_startXEdit.GetWindowTextW(sx);
	m_startYEdit.GetWindowTextW(sy);
	m_endXEdit.GetWindowTextW(ex);
	m_endYEdit.GetWindowTextW(ey);
	m_width.GetWindowTextW(w);
	m_height.GetWindowTextW(h);

	CPoint start(_ttoi(sx), _ttoi(sy));
	CPoint end(_ttoi(ex), _ttoi(ey));
	CSize motor(_ttoi(w), _ttoi(h));

	CRect curRect(start.x, start.y, end.x, end.y);
	curRect.NormalizeRect();  // �»��-���ϴ� ����

	/*if (curRect.Width() > m_logicalBounds.Width() || curRect.Height() > m_logicalBounds.Height()) {
		m_logicalBounds.SetRect(0, 0, max(curRect.Width() + 10, m_logicalBounds.Width() + 10), max(curRect.Height() + 10, m_logicalBounds.Height() + 10));
	}*/

	int motorX = m_radioXAxis.GetCheck() == BST_CHECKED ? (end.x - start.x) / 2 + start.x : end.x;
	int motorY = m_radioXAxis.GetCheck() == BST_CHECKED ? start.y : (end.y - start.y) / 2 + start.y;

	// üũ�ڽ� ���¿� ���� ���� ���� �߰� ���� ����
	BOOL bChecked = m_subMotorCheck.GetCheck();
	Motor* parentMotor = nullptr;

	if (bChecked == BST_CHECKED) {
		// ���� ���͸� �߰��Ϸ��� �θ� ���͸� �����ؾ� ��
		int selectedIndex = m_motorListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (selectedIndex != -1) {
			CString motorID = m_motorListCtrl.GetItemText(selectedIndex, 0);
			int motorIdInt = _ttoi(motorID);
			parentMotor = GetSelectedMotor(motorIdInt);

			if (!parentMotor) {
				AfxMessageBox(_T("���͸� �����ϼ���!"));
				return;
			}

			// ���� ���ʹ� �θ� ������ ���� ���� �־�� ��
			CPoint motorStart = parentMotor->motorPos - parentMotor->motorSize;
			CPoint motorEnd = parentMotor->motorPos + parentMotor->motorSize;

			// ���� ���Ͱ� �θ� ������ ���� ���� �־�� ���� üũ
			if (start.x < motorStart.x || start.y < motorStart.y ||
				end.x > motorEnd.x || end.y > motorEnd.y) {
				AfxMessageBox(_T("���� ���ʹ� ���� ������ ���� ���� �־�� �մϴ�."));
				return;
			}
		}
		else {
			AfxMessageBox(_T("���� ���͸� �߰��� �׸��� �����ϼ���!"));
			return;
		}
	}

	// ���� �߰�
	m_motorManager.AddMotor(
		parentMotor, // �θ� ���� (������ nullptr)
		m_radioXAxis.GetCheck() == BST_CHECKED,
		CPoint(start.x, start.y),
		CPoint(end.x, end.y),
		CPoint(motorX, motorY),
		CSize(motor.cx / 2, motor.cy / 2)
	);

	// ����Ʈ ��Ʈ�ѿ� ���� Ʈ�� ǥ��
	DisplayMotorTree(m_motorListCtrl, m_motorManager.rootMotors);

	// �ٽ� �׸���
	if (m_pParentView)
		m_pParentView->Invalidate();
}


Motor* MotorUI::GetSelectedMotor(int selectedIndex)
{
	// rootMotors���� �����Ͽ� Ʈ�� ������ ��������� Ž��
	for (Motor* rootMotor : m_motorManager.rootMotors)
	{
		Motor* selectedMotor = FindMotorByID(rootMotor, selectedIndex);
		if (selectedMotor) return selectedMotor;
	}
	return nullptr;
}

// ��������� ���͸� ã�� �Լ� (ID ��)
Motor* MotorUI::FindMotorByID(Motor* node, int selectedID)
{
	if (!node) return nullptr;

	if (node->m_id == selectedID)  // ���õ� ID�� ��ġ�ϸ� �� ���͸� ��ȯ
		return node;

	// �ڽĵ鿡 ���� ��� ȣ��
	for (Motor* child : node->children)
	{
		Motor* foundMotor = FindMotorByID(child, selectedID);
		if (foundMotor) return foundMotor;
	}

	return nullptr; // ã�� �������� null ��ȯ
}

void MotorUI::OnRemoveMotor()
{
	std::vector<int> selectedIndices;

	// ���� ���õ� �ε��� ����
	int index = -1;
	while ((index = m_motorListCtrl.GetNextItem(index, LVNI_SELECTED)) != -1)
	{
		selectedIndices.push_back(index);
	}

	if (selectedIndices.empty())
	{
		AfxMessageBox(_T("������ ���͸� �����ϼ���!"));
		return;
	}

	// ������ Motor ������ ����
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

	// ���� ó��
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

	// �ε��� �������� ����Ʈ���� ���� (index shift ����)
	std::sort(selectedIndices.rbegin(), selectedIndices.rend());
	for (int selectedIndex : selectedIndices)
	{
		m_motorListCtrl.DeleteItem(selectedIndex);
	}

	// �ٽ� �׸���
	if (m_pParentView)
		m_pParentView->Invalidate();
}

// ���� ���� ���
void MotorUI::OnSaveMotor()
{
	m_motorManager.SaveMotorData();
}

void MotorUI::OnLoadMotor()
{
	m_motorManager.LoadMotorData();
	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	m_motorListCtrl.DeleteAllItems();

	// ���� ����Ʈ�� �ִ� ��� ���͸� ����Ʈ ��Ʈ�ѿ� �߰�
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
	if (m_radioYAxis.GetSafeHwnd() && m_radioYAxis.GetCheck() == BST_CHECKED)  // Y�� ���� ��ư ���õǾ��� ��
	{
		CString startX;
		m_startXEdit.GetWindowTextW(startX);
		m_endXEdit.SetWindowTextW(startX);
	}
}

void MotorUI::OnChangeStartY()
{
	if (m_radioXAxis.GetSafeHwnd() && m_radioXAxis.GetCheck() == BST_CHECKED)  // X�� ���� ��ư ���õǾ��� ��
	{
		CString startY;
		m_startYEdit.GetWindowTextW(startY);
		m_endYEdit.SetWindowTextW(startY);
	}
}