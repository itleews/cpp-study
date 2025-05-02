#include "pch.h"
#include "MotorUI.h"
#include "ChildView.h"

void MotorUI::CreateUI(CWnd* pParent)
{
    // ����Ʈ ��Ʈ�� ����
    m_motorListCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, CRect(0, 0, 0, 0), pParent, 1);
    m_motorListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER);
    m_motorListCtrl.InsertColumn(0, _T("���� ID"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(1, _T("���� ��"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(2, _T("���� ��ġ"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(3, _T("�� ��ġ"), LVCFMT_LEFT, 100);
    m_motorListCtrl.InsertColumn(4, _T("���� ��ġ"), LVCFMT_LEFT, 100);

    // �׷�ڽ� ����
    m_groupInput.Create(_T("��ġ �� ũ�� ����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), pParent, 3000);
    m_groupButtons.Create(_T("���� ����"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), pParent, 3001);
	m_groupControl.Create(_T("���� ���ۺ�"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(0, 0, 0, 0), pParent, 3002);

    // �Է� �ʵ� ����
    m_startXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2001);
    m_startXEdit.SetWindowTextW(_T("0"));

    m_startYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2002);
    m_startYEdit.SetWindowTextW(_T("100"));

    m_endXEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2003);
    m_endXEdit.SetWindowTextW(_T("1000"));

    m_endYEdit.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2004);
    m_endYEdit.SetWindowTextW(_T("100"));

    m_width.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2005);
    m_width.SetWindowTextW(_T("100"));

    m_height.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, CRect(0, 0, 0, 0), pParent, 2006);
    m_height.SetWindowTextW(_T("100"));

	m_subMotorCheck.Create(_T("���� ���ͷ� �߰�"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(0, 0, 0, 0), pParent, 2007);
	m_subMotorCheck.SetCheck(BST_UNCHECKED);

    // ���� ��ư ����
    m_radioXAxis.Create(_T("X��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), pParent, 3003);
    m_radioYAxis.Create(_T("Y��"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, CRect(0, 0, 0, 0), pParent, 3004);
    m_radioXAxis.SetCheck(TRUE);  // �⺻���� X�� ����
    m_endYEdit.EnableWindow(FALSE);  // Y�� ��Ȱ��ȭ

    // �� ����
    m_labelStart.Create(_T("���� ��ġ(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent);
    m_labelEnd.Create(_T("�� ��ġ(x, y)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent);
    m_labelSize.Create(_T("ũ��(W, H)"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent);
    m_labelAxis.Create(_T("���� ��"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent);

    // ���� ���� ��ư ����
    m_addMotorButton.Create(_T("�߰�"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, 1001);
    m_removeMotorButton.Create(_T("����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, 1002);
    m_saveMotorButton.Create(_T("����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, 1003);
    m_loadMotorButton.Create(_T("�ҷ�����"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, 1004);
}

void MotorUI::SetPositionUI(CRect& drawArea, int cx, int cy)
{
    // UI ��Ʈ�ѵ��� ��ġ�� �׸��忡 �°� ��ġ
	const int margin = 10;
	const int sectionGap = 20;

	const int btnHeight = 30;
	const int btnGap = 10;

	const int editWidth = 100;
	const int editHeight = 25;

	int rightWidth = cx - drawArea.right - 2 * margin;
	int motorAreaWidth = cx - rightWidth - 2 * margin;
	int motorAreaHeight = cy - 2 * margin;

	int rightX = motorAreaWidth + margin;

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
	CRect controlRect(rightX, controlTop, rightX + rightWidth, controlTop + controlHeight);

	m_motorControlPanel.UpdateLayout(controlRect);

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

