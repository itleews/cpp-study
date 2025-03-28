#include "pch.h"
#include "Brick.h"

void Brick::Draw(CDC* pDC) {
    if (!isBroken) {
        CBrush brush(RGB(0, 255, 0));  // 초록색으로 벽돌 그리기
        pDC->SelectObject(&brush);
        pDC->Rectangle(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
    }
}

bool Brick::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x - width / 2, y - height / 2, x + width / 2, y + height / 2);

    // pWnd가 nullptr인지 확인한 후 무효화
    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);  // CWnd를 통해 무효화 요청
    }

	return TRUE;
}

void Brick::Break() {
    isBroken = true;
}
