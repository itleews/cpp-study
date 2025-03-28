#pragma once
#include "GameObject.h"

class Ball : public GameObject {
public:
    int m_x, m_y;
    int m_radius;
    int m_dx, m_dy;
    COLORREF m_ballColor;

    Ball(int x, int y, int radius, int dx, int dy) : 
        m_x(x), m_y(y), m_radius(radius), m_dx(dx), m_dy(dy), m_ballColor(RGB(255, 255, 0)) {
    }

    void Draw(CDC* pDC) override;
    bool Update(CRect boundary, CWnd* pWnd) override;
};
