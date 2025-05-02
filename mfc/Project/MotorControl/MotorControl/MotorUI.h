#pragma once
#include <afxwin.h>
#include "MotorManager.h"
#include "MotorControlPanel.h"

class MotorUI : public CWnd {
public:
    // UI 컨트롤 선언
    CListCtrl m_motorListCtrl;
    CButton m_addMotorButton, m_removeMotorButton, m_saveMotorButton, m_loadMotorButton, m_subMotorCheck;
    CButton m_groupInput, m_groupButtons, m_groupControl;
    CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit, m_width, m_height;
    CButton m_radioXAxis, m_radioYAxis;
    CStatic m_labelStart, m_labelEnd, m_labelSize, m_labelAxis;
	MotorControlPanel m_motorControlPanel;

    void DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors);
    void DisplayMotorRecursive(CListCtrl& listCtrl, Motor* node, int depth, int& itemIndex);

protected:
    // UI 생성 함수
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
