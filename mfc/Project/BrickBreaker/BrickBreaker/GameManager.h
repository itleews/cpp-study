#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>
#include <cmath>

class GameManager {
public:
    int m_brickCount;
    int ballCount;

    std::vector<Ball> balls;
    std::vector<Paddle> paddles;
    std::vector<Brick> bricks;

    void StartGame(CRect boundary, CWnd* pWnd);
    void DrawGame(CDC* pDC);
    void HandleCollisions(CWnd* pWnd);
    void EndGame(CWnd* pWnd);
    void ResetGame(const CRect& boundary, CWnd* pWnd);
	void ClearGame(CWnd* pWnd);
    void DestroyBall(Ball* ball);
    double clamp(double value, double min, double max) {
        return max(min, min(value, max));
    }
};
