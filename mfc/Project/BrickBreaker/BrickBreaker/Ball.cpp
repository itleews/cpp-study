#include "pch.h"
#include "Ball.h"

void Ball::Draw(CDC* pDC) {
    // 브러시 생성 및 선택
    CBrush brush(m_ballColor);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // 원 그리기
    pDC->Ellipse(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // 이전 브러시 복원
    pDC->SelectObject(pOldBrush);
}


bool Ball::Update(CRect boundary, CWnd* pWnd) {
    // 원의 이전 위치를 저장
    CRect oldRect(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // 위치 이동
    m_x += m_dx;
    m_y += m_dy;

    // 경계 체크 및 이동 반영
    if (m_x + m_radius + m_dx >= boundary.right) {
        m_dx = -m_dx;
        m_x = boundary.right - m_radius;
    }
    else if (m_x - m_radius + m_dx <= boundary.left) {
        m_dx = -m_dx;
        m_x = boundary.left + m_radius;
    }
    if (m_y + m_radius >= boundary.bottom) {
        return FALSE;
    }
    else if (m_y - m_radius + m_dy <= boundary.top) {
        m_dy = -m_dy;
        m_y = boundary.top + m_radius;
    }

    // 새로운 위치를 계산
    CRect newRect(m_x - m_radius, m_y - m_radius, m_x + m_radius, m_y + m_radius);

    // 이전 위치와 새로운 위치를 포함하는 영역으로 합침
    oldRect.UnionRect(&oldRect, &newRect);

    // pWnd가 nullptr인지 확인한 후 무효화
    if (pWnd != nullptr) {
        pWnd->InvalidateRect(&oldRect, FALSE);  // CWnd를 통해 무효화 요청
    }
}