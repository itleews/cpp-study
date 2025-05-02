#pragma once

class MotorControlPanel : public CWnd {
public:
    CButton m_buttonUp;  // ↑ 버튼
    CButton m_buttonDown;  // ↓ 버튼
    CButton m_buttonLeft;  // ← 버튼
    CButton m_buttonRight;  // → 버튼

    void CreatePanel(CWnd* pParent);
    void UpdateLayout(const CRect& area);  // 레이아웃 업데이트 메소드

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonUpClicked();  // ↑ 버튼 클릭 이벤트 핸들러
    afx_msg void OnButtonDownClicked();  // ↓ 버튼 클릭 이벤트 핸들러
	afx_msg void OnButtonLeftClicked();  // ← 버튼 클릭 이벤트 핸들러 
	afx_msg void OnButtonRightClicked();  // → 버튼 클릭 이벤트 핸들러
};