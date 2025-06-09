#include "pch.h"
#include "MotorListPanel.h"
#include "MotorUI.h"

CString AxisTypeToString(AxisType axis) {
	switch (axis) {
	case AxisType::X: return _T("X");
	case AxisType::Y: return _T("Y");
	case AxisType::T: return _T("T");
	default: return _T("Unknown");
	}
}

void MotorListPanel::BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows)
{
	if (!node) return;

	CString indent;
	for (int i = 0; i < depth; ++i)
		indent += _T("    ");

	CString idStr;
	idStr.Format(_T("%d"), node->m_id);

	CString typeStr = AxisTypeToString(node->axis);

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

	if (!node)
		return;
	
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
