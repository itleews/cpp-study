#include "pch.h"
#include "MotorControlPanel.h"

BEGIN_MESSAGE_MAP(MotorControlPanel, CWnd)
	ON_WM_CREATE()
    ON_BN_CLICKED(1, &MotorControlPanel::OnButtonUpClicked)
    ON_BN_CLICKED(2, &MotorControlPanel::OnButtonDownClicked)
    ON_BN_CLICKED(3, &MotorControlPanel::OnButtonLeftClicked)
    ON_BN_CLICKED(4, &MotorControlPanel::OnButtonRightClicked)
END_MESSAGE_MAP()

int MotorControlPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // ��ư ���� (this�� �θ�!)
    m_buttonUp.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(50, 20, 110, 80), this, 1);
    m_buttonDown.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(50, 90, 110, 150), this, 2);
    m_buttonLeft.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(10, 90, 70, 150), this, 3);
    m_buttonRight.Create(_T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(90, 90, 150, 150), this, 4);
    return 0;
}

void MotorControlPanel::UpdateLayout(const CRect& area) {
    int keySize = 60;
    int spacing = 10;
    int keyCenterX = (area.left + area.right) / 2;
    int keyTopY = area.top + 20;

    // ��ư ��ġ ��� (��/�Ʒ�/����/������)
    m_buttonUp.SetWindowPos(nullptr, keyCenterX - keySize / 2, keyTopY, keySize, keySize, SWP_NOZORDER);
    m_buttonDown.SetWindowPos(nullptr, keyCenterX - keySize / 2, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
    m_buttonLeft.SetWindowPos(nullptr, keyCenterX - keySize * 1.5 - spacing, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
    m_buttonRight.SetWindowPos(nullptr, keyCenterX + keySize / 2 + spacing, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
}

void MotorControlPanel::OnButtonUpClicked() {
    AfxMessageBox(_T("Up button clicked"));
    // �� ��ư Ŭ�� ó��
}

void MotorControlPanel::OnButtonDownClicked() {
    AfxMessageBox(_T("Down button clicked"));
    // �Ʒ� ��ư Ŭ�� ó��
}

void MotorControlPanel::OnButtonLeftClicked() {
    AfxMessageBox(_T("Left button clicked"));
    // ���� ��ư Ŭ�� ó��
}

void MotorControlPanel::OnButtonRightClicked() {
    AfxMessageBox(_T("Right button clicked"));
    // ������ ��ư Ŭ�� ó��
}