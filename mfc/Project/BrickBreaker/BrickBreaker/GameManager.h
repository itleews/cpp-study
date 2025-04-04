#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>

class GameManager {
public:
    int brickCount;
    int ballCount;

    std::vector<Ball> balls;
    std::vector<Paddle> paddles;
    std::vector<Brick> bricks;

    void StartGame(CRect boundary, CWnd* pWnd);
    void DrawGame(CDC* pDC);
    void HandleCollisions(CWnd* pWnd);
    void EndGame(CWnd* pWnd);
    void ResetGame(const CRect& boundary, CWnd* pWnd);
    void DestroyBall(Ball* ball);
};
