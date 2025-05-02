#pragma once

class MotorControlPanel : public CWnd {
public:
    MotorControlPanel();
    ~MotorControlPanel();

    BOOL Create(CWnd* pParent, const CRect& area);
    void Draw(CDC* pDC);
    // Ű �ڽ� ��ġ ����
    void UpdateLayout(const CRect& area);

private:
    CRect m_keyRects[4];       // ����Ű ��ġ
    CString m_keyLabels[4];    // �ؽ�Ʈ: ��, ��, ��, ��
    CFont m_keyFont;
    CRect m_panelArea;
    bool m_bIsButtonClicked[4];

    void InitResources();      // ��Ʈ �� �ʱ�ȭ

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
