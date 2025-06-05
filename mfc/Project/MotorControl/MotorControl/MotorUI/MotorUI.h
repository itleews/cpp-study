#pragma once
#include <afxwin.h>
#include "../Motor/MotorManager.h"
#include "../Motor/MotorTransform.h"
#include "MotorListPanel.h"
#include "MotorPreviewPanel.h"
#include "MotorMovePanel.h"

class CChildView;

class MotorUI : public CWnd {
public:
    MotorManager m_motorManager;
	MotorListPanel m_motorListPanel;
	MotorPreviewPanel m_motorPreviewPanel;
	MotorMovePanel m_motorMovePanel;
	MotorTransform m_motorTransform;
    
	bool m_isAddMotorMode = false;
	bool m_isAddSubmotorMode = false;
	bool m_isAddRotatingMotorMode = false;
	bool m_isSaveMotorMode = false;
    Motor* m_selectedMotor = nullptr;
	Motor m_previewMotor; // 미리보기용 모터
    
    int GetSelectedMotorId() const;
    Motor* GetSelectedMotor(int selectedIndex);
    Motor* FindMotorByID(Motor* node, int selectedID);
	void DeleteMotorRecursive(Motor* motor);
    void MoveSelectedAxis(int dx, int dy);
    void UpdateMotorListTexts();
    void SetParentView(CChildView* pView) { m_pParentView = pView; }
    
private:
    CMFCListCtrl m_motorListCtrl;
    CButton m_addMotorBtn, m_removeMotorBtn, m_saveMotorBtn, m_loadMotorBtn, m_addSubMotorBtn, m_addRotMotorBtn;
    CButton m_groupInput, m_groupButtons, m_groupControl;
    std::vector<CEdit*> m_numericEdits;
    CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit, m_width, m_height, m_speed;
    CButton m_radioXAxis, m_radioYAxis;
    CButton m_controlBtn[4]; // 조작부 버튼들
    CStatic m_labelStart, m_labelEnd, m_labelSize, m_labelAxis, m_labelSpeed;

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
    void UpdatePreviewData();
    void ResetMotorUI();
    CString FilterNumericInput(const CString& input);

protected:
    // UI 생성 함수
    CChildView* m_pParentView = nullptr;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnAddMotor();
	afx_msg void OnAddSubMotor();
    afx_msg void OnAddRotatingMotor();
    afx_msg void OnRemoveMotor();
    afx_msg void OnSaveMotor();
    afx_msg void OnLoadMotor();
    afx_msg void OnBnClickedRadioXAxis();
    afx_msg void OnBnClickedRadioYAxis();
    afx_msg void OnEditChanged();
    afx_msg void OnChangeStartX();
    afx_msg void OnChangeStartY();
    afx_msg void OnNMClickMotorList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedControlUpButton();
	afx_msg void OnBnClickedControlDownButton();
	afx_msg void OnBnClickedControlLeftButton();
	afx_msg void OnBnClickedControlRightButton();
    DECLARE_MESSAGE_MAP()
};
