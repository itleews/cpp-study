#pragma once
#include "GameObject.h"

class Brick : public GameObject {
public:
    bool isBroken;

    Brick(int x, int y, int width, int height)
        : GameObject(x, y, width, height), isBroken(false) {
    }

    void Draw(CDC* pDC) override;
    bool Update(CRect boundary, CWnd* pWnd) override;
    void Break();
};
