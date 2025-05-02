#pragma once

class MotorControlPanel : public CWnd {
public:
    MotorControlPanel();
    ~MotorControlPanel();

    BOOL Create(CWnd* pParent, const CRect& area);
    void Draw(CDC* pDC);
    // 키 박스 위치 계산용
    void UpdateLayout(const CRect& area);

private:
    CRect m_keyRects[4];       // 방향키 위치
    CString m_keyLabels[4];    // 텍스트: ▲, ▽, ◀, ▶
    CFont m_keyFont;
    CRect m_panelArea;
    bool m_bIsButtonClicked[4];

    void InitResources();      // 폰트 등 초기화

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
