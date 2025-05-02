#pragma once

class MotorControlPanel : public CWnd {
public:
    CButton m_buttonUp;  // �� ��ư
    CButton m_buttonDown;  // �� ��ư
    CButton m_buttonLeft;  // �� ��ư
    CButton m_buttonRight;  // �� ��ư

    void CreatePanel(CWnd* pParent);
    void UpdateLayout(const CRect& area);  // ���̾ƿ� ������Ʈ �޼ҵ�

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonUpClicked();  // �� ��ư Ŭ�� �̺�Ʈ �ڵ鷯
    afx_msg void OnButtonDownClicked();  // �� ��ư Ŭ�� �̺�Ʈ �ڵ鷯
	afx_msg void OnButtonLeftClicked();  // �� ��ư Ŭ�� �̺�Ʈ �ڵ鷯 
	afx_msg void OnButtonRightClicked();  // �� ��ư Ŭ�� �̺�Ʈ �ڵ鷯
};