// Brick.cpp
#include "pch.h"
#include "Brick.h"
#include <afxwin.h>

void Brick::Draw(CDC* pDC) {
    if (!isBroken) {
        CBrush brush(color);
        pDC->SelectObject(&brush);
        pDC->Rectangle(x, y, x + width, y + height);
    }
}

bool Brick::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x, y, x + width, y + height);

    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);
    }

    return TRUE;
}

void Brick::Break() {
    if (hitCount > 0) {
        hitCount--;  // 내구도 감소

        // 색상 밝기 조절 (남은 hitCount에 따라 점점 밝아짐)
        if (hitCount > 0) {
            int r = 0, g = 0, b = 0;
            switch (hitCount) {
            case 1:
                r = 255; g = 99; b = 71;
                break;
            case 2:
                r = 255; g = 0; b = 0;
                break;
            }

            color = RGB(r, g, b); // 색상 업데이트
        }
        else if (hitCount == 0) {
            isBroken = true;  // 벽돌이 완전히 부서짐
        }
    }
}