#pragma once
#include "../Motor/MotorManager.h"
#include <vector>

struct DisplayRow{
    CString id;
    CString type;
    CString strPos;
    CString endPos;
    CString relMotorPos;
    CString absMotorPos;
};

class MotorListPanel : public CWnd {
public:
    CMFCListCtrl m_motorListCtrl;
	MotorManager m_motorManager;
private:
    void CreateListControl();
	void DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors);
	void BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows);
	void UpdateMotorListTexts();
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()
};