#pragma once
#include <afxwin.h>

class MotorUI {
public:
    // UI ��Ʈ�� ����
    CListCtrl m_motorListCtrl;
    CButton m_addMotorButton, m_removeMotorButton, m_saveMotorButton, m_loadMotorButton;
    CButton m_groupInput, m_groupButtons, m_groupControl;
    CEdit m_startXEdit, m_startYEdit, m_endXEdit, m_endYEdit, m_width, m_height;
    CButton m_radioXAxis, m_radioYAxis;
    CStatic m_labelStart, m_labelEnd, m_labelSize, m_labelAxis;

    // UI ���� �Լ�
    void CreateUI(CWnd* pParent);
    void SetPositionUI(CRect& drawArea, int cx, int cy);
};
