#include "pch.h"
#include "MotorControlPanel.h"

BEGIN_MESSAGE_MAP(MotorControlPanel, CWnd)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

MotorControlPanel::MotorControlPanel() {
    m_keyLabels[0] = _T("▲");
    m_keyLabels[1] = _T("▼");
    m_keyLabels[2] = _T("◀");
    m_keyLabels[3] = _T("▶");
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
    _tcscpy_s(lf.lfFaceName, _T("맑은 고딕"));
    m_keyFont.CreateFontIndirect(&lf);
}

// Create 메소드 활성화
BOOL MotorControlPanel::Create(CWnd* pParent, const CRect& area) {
    m_panelArea = area;

    // 윈도우 클래스 등록
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(nullptr, IDC_ARROW));

    // 실제 윈도우 생성
    BOOL result = CWnd::CreateEx(0, className, _T("MotorControlPanel"), WS_CHILD | WS_VISIBLE, area, pParent, 1234);
    if (result) {
        UpdateLayout(area); // 생성 후 레이아웃 업데이트
    }
    return result;
}

// SetWindowPos 호출 후 화면 갱신
void MotorControlPanel::UpdateLayout(const CRect& area) {
    int keySize = 60;
    int spacing = 10;
    int keyCenterX = (area.left + area.right) / 2;
    int keyTopY = area.top + 20;

    m_keyRects[0] = CRect(keyCenterX - keySize / 2, keyTopY, keyCenterX + keySize / 2, keyTopY + keySize); // ↑
    m_keyRects[1] = CRect(keyCenterX - keySize / 2, keyTopY + keySize + spacing, keyCenterX + keySize / 2, keyTopY + keySize * 2 + spacing); // ↓
    m_keyRects[2] = CRect(keyCenterX - keySize * 1.5 - spacing, keyTopY + keySize + spacing, keyCenterX - keySize / 2 - spacing, keyTopY + keySize * 2 + spacing); // ←
    m_keyRects[3] = CRect(keyCenterX + keySize / 2 + spacing, keyTopY + keySize + spacing, keyCenterX + keySize * 1.5 + spacing, keyTopY + keySize * 2 + spacing); // →

    // 레이아웃을 업데이트 후, 화면 갱신
    Invalidate();  // 화면을 무효화
    UpdateWindow(); // 화면을 갱신
}

void MotorControlPanel::Draw(CDC* pDC) {
    CFont* pOldFont = pDC->SelectObject(&m_keyFont);

    for (int i = 0; i < 4; ++i) {
        // 클릭된 버튼은 다른 색으로 표시
        COLORREF brushColor = m_bIsButtonClicked[i] ? RGB(200, 200, 255) : RGB(255, 255, 255);  // 클릭된 버튼은 다른 색
        CBrush brush(brushColor);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        // 라운드 사각형 그리기
        pDC->RoundRect(&m_keyRects[i], CPoint(10, 10));
        pDC->SelectObject(pOldBrush);

        // 텍스트 설정
        pDC->SetBkMode(TRANSPARENT);  // 배경을 투명하게 설정
        pDC->SetTextColor(RGB(0, 0, 0));  // 텍스트 색상 설정

        // 텍스트 위치 계산
        CSize textSize = pDC->GetTextExtent(m_keyLabels[i]);
        int textX = m_keyRects[i].left + (m_keyRects[i].Width() - textSize.cx) / 2;
        int textY = m_keyRects[i].top + (m_keyRects[i].Height() - textSize.cy) / 2;

        // 텍스트 그리기
        pDC->TextOutW(textX, textY, m_keyLabels[i]);
    }

    pDC->SelectObject(pOldFont);
}

void MotorControlPanel::OnLButtonDown(UINT nFlags, CPoint point) {
    // 마우스 클릭 위치가 버튼 안에 있으면 버튼 클릭 상태를 토글
    for (int i = 0; i < 4; ++i) {
        if (m_keyRects[i].PtInRect(point)) {
            m_bIsButtonClicked[i] = !m_bIsButtonClicked[i];
            Invalidate();  // 화면 갱신
            break;
        }
    }
}