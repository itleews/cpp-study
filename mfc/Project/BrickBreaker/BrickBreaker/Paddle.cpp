#include "pch.h"
#include "Paddle.h"

void Paddle::Draw(CDC* pDC) {
    CBrush brush(RGB(45, 52, 54));  // 색상 설정
    pDC->SelectObject(&brush);  // 브러시 선택

    // 둥근 모서리를 가진 사각형 그리기
    pDC->RoundRect(x, y, x + width, y + height, 20, 20);  // 20은 모서리 반지름
}

bool Paddle::Update(CRect boundary, CWnd* pWnd) {
    CRect oldRect(x - dx, y - dy, x + width + dx, y + height + dy);  // 이전 위치 확장

    dx = 30;  // 패들의 이동 속도
    dy = 30;

    int HeightLimit = boundary.Height() - boundary.Height() / 5;  // 높이 제한

    if (movingLeft && boundary.left < x - dx) {
        x -= dx;
    }
    else if (movingRight && boundary.right > x + width + dx) {
        x += dx;
    }
    if (movingUp && y - dy > HeightLimit) {
        y -= dy;
    }
    else if (movingDown && boundary.bottom > y + height + dy) {
        y += dy;
    }

    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);
    }

    return TRUE;
}