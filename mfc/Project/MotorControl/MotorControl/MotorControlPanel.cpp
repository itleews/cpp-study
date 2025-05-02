#include "pch.h"
#include "MotorControlPanel.h"

BEGIN_MESSAGE_MAP(MotorControlPanel, CWnd)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

MotorControlPanel::MotorControlPanel() {
    m_keyLabels[0] = _T("��");
    m_keyLabels[1] = _T("��");
    m_keyLabels[2] = _T("��");
    m_keyLabels[3] = _T("��");
    memset(m_bIsButtonClicked, 0, sizeof(m_bIsButtonClicked));
    InitResources();
}

MotorControlPanel::~MotorControlPanel() {
    if (m_keyFont.GetSafeHandle())
        m_keyFont.DeleteObject();
}

void MotorControlPanel::InitResources() {
    LOGFONT lf = { 0 };
    lf.lfHeight = 40;
    _tcscpy_s(lf.lfFaceName, _T("���� ���"));
    m_keyFont.CreateFontIndirect(&lf);
}

// Create �޼ҵ� Ȱ��ȭ
BOOL MotorControlPanel::Create(CWnd* pParent, const CRect& area) {
    m_panelArea = area;

    // ������ Ŭ���� ���
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(nullptr, IDC_ARROW));

    // ���� ������ ����
    BOOL result = CWnd::CreateEx(0, className, _T("MotorControlPanel"), WS_CHILD | WS_VISIBLE, area, pParent, 1234);
    if (result) {
        UpdateLayout(area); // ���� �� ���̾ƿ� ������Ʈ
    }
    return result;
}

// SetWindowPos ȣ�� �� ȭ�� ����
void MotorControlPanel::UpdateLayout(const CRect& area) {
    int keySize = 60;
    int spacing = 10;
    int keyCenterX = (area.left + area.right) / 2;
    int keyTopY = area.top + 20;

    m_keyRects[0] = CRect(keyCenterX - keySize / 2, keyTopY, keyCenterX + keySize / 2, keyTopY + keySize); // ��
    m_keyRects[1] = CRect(keyCenterX - keySize / 2, keyTopY + keySize + spacing, keyCenterX + keySize / 2, keyTopY + keySize * 2 + spacing); // ��
    m_keyRects[2] = CRect(keyCenterX - keySize * 1.5 - spacing, keyTopY + keySize + spacing, keyCenterX - keySize / 2 - spacing, keyTopY + keySize * 2 + spacing); // ��
    m_keyRects[3] = CRect(keyCenterX + keySize / 2 + spacing, keyTopY + keySize + spacing, keyCenterX + keySize * 1.5 + spacing, keyTopY + keySize * 2 + spacing); // ��

    // ���̾ƿ��� ������Ʈ ��, ȭ�� ����
    Invalidate();  // ȭ���� ��ȿȭ
    UpdateWindow(); // ȭ���� ����
}

void MotorControlPanel::Draw(CDC* pDC) {
    CFont* pOldFont = pDC->SelectObject(&m_keyFont);

    for (int i = 0; i < 4; ++i) {
        // Ŭ���� ��ư�� �ٸ� ������ ǥ��
        COLORREF brushColor = m_bIsButtonClicked[i] ? RGB(200, 200, 255) : RGB(255, 255, 255);  // Ŭ���� ��ư�� �ٸ� ��
        CBrush brush(brushColor);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        // ���� �簢�� �׸���
        pDC->RoundRect(&m_keyRects[i], CPoint(10, 10));
        pDC->SelectObject(pOldBrush);

        // �ؽ�Ʈ ����
        pDC->SetBkMode(TRANSPARENT);  // ����� �����ϰ� ����
        pDC->SetTextColor(RGB(0, 0, 0));  // �ؽ�Ʈ ���� ����

        // �ؽ�Ʈ ��ġ ���
        CSize textSize = pDC->GetTextExtent(m_keyLabels[i]);
        int textX = m_keyRects[i].left + (m_keyRects[i].Width() - textSize.cx) / 2;
        int textY = m_keyRects[i].top + (m_keyRects[i].Height() - textSize.cy) / 2;

        // �ؽ�Ʈ �׸���
        pDC->TextOutW(textX, textY, m_keyLabels[i]);
    }

    pDC->SelectObject(pOldFont);
}

void MotorControlPanel::OnLButtonDown(UINT nFlags, CPoint point) {
    // ���콺 Ŭ�� ��ġ�� ��ư �ȿ� ������ ��ư Ŭ�� ���¸� ���
    for (int i = 0; i < 4; ++i) {
        if (m_keyRects[i].PtInRect(point)) {
            m_bIsButtonClicked[i] = !m_bIsButtonClicked[i];
            Invalidate();  // ȭ�� ����
            break;
        }
    }
}