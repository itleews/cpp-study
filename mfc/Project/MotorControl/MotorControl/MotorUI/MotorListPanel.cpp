#include "pch.h"
#include "MotorListPanel.h"
#include "MotorUI.h"

BEGIN_MESSAGE_MAP(MotorListPanel, CWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

int MotorListPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateListControl();
	SetTimer(2, 16, NULL);
	return 0;
}

void MotorListPanel::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2)
	{
		if (!m_motorUI.m_isAddSubmotorMode)
			UpdateMotorListTexts();
	}

	CWnd::OnTimer(nIDEvent);
}

void MotorListPanel::OnDestroy()
{
	KillTimer(2);  // 타이머 ID 1 제거
	CWnd::OnDestroy();
}

void MotorListPanel::BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows)
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

void MotorListPanel::DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors)
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

void MotorListPanel::UpdateMotorListTexts()
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

void MotorListPanel::CreateListControl()
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