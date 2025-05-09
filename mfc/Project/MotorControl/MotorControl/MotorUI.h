#pragma once
#include <afxwin.h>
#include "MotorManager.h"

class CChildView;

class MotorUI : public CWnd {
public:
    // UI 컨트롤 선언
    MotorManager m_motorManager;
    CListCtrl m_motorListCtrl;
    CButton m_addMotorButton, m_removeMotorButton, m_saveMotorButton, m_loadMotorButton, m_subMotorCheck;
    CButton m_groupInput, m_groupButtons, m_groupControl;
    CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit, m_width, m_height;
    CButton m_radioXAxis, m_radioYAxis;
	CButton m_controlButton[4]; // 조작부 버튼들
    CStatic m_labelStart, m_labelEnd, m_labelSize, m_labelAxis;

    void SetParentView(CChildView* pView) { m_pParentView = pView; }
    void DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors);
    void DisplayMotorRecursive(CListCtrl& listCtrl, Motor* node, int depth, int& itemIndex);
    Motor* GetSelectedMotor(int selectedIndex);
    Motor* FindMotorByID(Motor* node, int selectedID);
	void DeleteMotorRecursive(Motor* motor);

protected:
    // UI 생성 함수
    CChildView* m_pParentView = nullptr;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnAddMotor();
    afx_msg void OnRemoveMotor();
    afx_msg void OnSaveMotor();
    afx_msg void OnLoadMotor();
    afx_msg void OnBnClickedRadioXAxis();
    afx_msg void OnBnClickedRadioYAxis();
    afx_msg void OnChangeStartX();
    afx_msg void OnChangeStartY();
    DECLARE_MESSAGE_MAP()
};
