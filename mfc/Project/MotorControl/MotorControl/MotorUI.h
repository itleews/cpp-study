#pragma once
#include <afxwin.h>
#include "MotorManager.h"

class CChildView;


struct DisplayRow {
    CString id;
    CString type;
    CString strPos;
    CString endPos;
    CString relMotorPos;
    CString absMotorPos;
};

class MotorUI : public CWnd {
public:
    // UI 컨트롤 선언
    MotorManager m_motorManager;
    CMFCListCtrl m_motorListCtrl;
    CButton m_addMotorButton, m_removeMotorButton, m_saveMotorButton, m_loadMotorButton, m_addSubMotorButton;
    CButton m_groupInput, m_groupButtons, m_groupControl;
    CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit, m_width, m_height, m_speed;
    CButton m_radioXAxis, m_radioYAxis;
	CButton m_controlButton[4]; // 조작부 버튼들
    CStatic m_labelStart, m_labelEnd, m_labelSize, m_labelAxis, m_labelSpeed;
	bool m_isAddSubmotorMode = false;
    CRect m_selectedMotorRect;

    void CreateListControl();
    void CreateInputFields();
	void CreateGroupBoxes();
	void CreateRadioButtons();
    void CreateLabels();
    void CreateMotorButtons();
    void CreateControlButtons();
    void CreateEdit(CEdit& edit, int id, const CString& defaultText);
    void CreateStatic(CStatic& label, const CString& text);
	void CreateButton(CButton& button, int id, const CString& text);

    void SetParentView(CChildView* pView) { m_pParentView = pView; }
    void DisplayMotorTree(CListCtrl& listCtrl, const std::vector<Motor*>& rootMotors);
    void BuildDisplayRows(Motor* node, int depth, const CPoint& parentOrigin, std::vector<DisplayRow>& outRows);
    Motor* GetSelectedMotor(int selectedIndex);
    Motor* FindMotorByID(Motor* node, int selectedID);
	void DeleteMotorRecursive(Motor* motor);
    void MoveSelectedAxis(int dx, int dy);
    void OnDestroy();
    void UpdateMotorListTexts();
    void MoveMotorRecursive(Motor* motor, int dx, int dy);

    CPoint SubToLogical(CPoint subMotorPos, CPoint mainMotorPos) {
		// 하위 모터의 위치를 부모 모터의 위치로 변환
		return CPoint(subMotorPos + mainMotorPos);
    }

protected:
    // UI 생성 함수
    CChildView* m_pParentView = nullptr;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnAddMotor();
	afx_msg void OnAddSubMotor();
    afx_msg void OnRemoveMotor();
    afx_msg void OnSaveMotor();
    afx_msg void OnLoadMotor();
    afx_msg void OnBnClickedRadioXAxis();
    afx_msg void OnBnClickedRadioYAxis();
    afx_msg void OnChangeStartX();
    afx_msg void OnChangeStartY();
    afx_msg void OnLvnItemChangedMotorList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedControlUpButton();
	afx_msg void OnBnClickedControlDownButton();
	afx_msg void OnBnClickedControlLeftButton();
	afx_msg void OnBnClickedControlRightButton();
    DECLARE_MESSAGE_MAP()
};
