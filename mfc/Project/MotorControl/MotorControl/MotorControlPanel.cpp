#include "pch.h"
#include "MotorControlPanel.h"

BEGIN_MESSAGE_MAP(MotorControlPanel, CWnd)
    ON_BN_CLICKED(1, &MotorControlPanel::OnButtonUpClicked)
    ON_BN_CLICKED(2, &MotorControlPanel::OnButtonDownClicked)
    ON_BN_CLICKED(3, &MotorControlPanel::OnButtonLeftClicked)
    ON_BN_CLICKED(4, &MotorControlPanel::OnButtonRightClicked)
END_MESSAGE_MAP()

void MotorControlPanel::CreatePanel(CWnd* pParent) {
    m_buttonUp.Create(_T("▲"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), pParent, 1);
    m_buttonDown.Create(_T("▼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), pParent, 2);
    m_buttonLeft.Create(_T("◀"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), pParent, 3);
    m_buttonRight.Create(_T("▶"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 0, 0), pParent, 4);
}

void MotorControlPanel::UpdateLayout(const CRect& area) {
    int keySize = 60;
    int spacing = 10;
    int keyCenterX = (area.left + area.right) / 2;
    int keyTopY = area.top + 20;

    // 버튼 위치 계산 (위/아래/왼쪽/오른쪽)
    m_buttonUp.SetWindowPos(nullptr, keyCenterX - keySize / 2, keyTopY, keySize, keySize, SWP_NOZORDER);
    m_buttonDown.SetWindowPos(nullptr, keyCenterX - keySize / 2, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
    m_buttonLeft.SetWindowPos(nullptr, keyCenterX - keySize * 1.5 - spacing, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
    m_buttonRight.SetWindowPos(nullptr, keyCenterX + keySize / 2 + spacing, keyTopY + keySize + spacing, keySize, keySize, SWP_NOZORDER);
}

void MotorControlPanel::OnButtonUpClicked() {
    AfxMessageBox(_T("Up button clicked"));
    // 위 버튼 클릭 처리
}

void MotorControlPanel::OnButtonDownClicked() {
    AfxMessageBox(_T("Down button clicked"));
    // 아래 버튼 클릭 처리
}

void MotorControlPanel::OnButtonLeftClicked() {
    AfxMessageBox(_T("Left button clicked"));
    // 왼쪽 버튼 클릭 처리
}

void MotorControlPanel::OnButtonRightClicked() {
    AfxMessageBox(_T("Right button clicked"));
    // 오른쪽 버튼 클릭 처리
}