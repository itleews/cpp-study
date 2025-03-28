#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>

class GameManager {
public:
    std::vector<Ball> balls;
    std::vector<Paddle> paddles;
    std::vector<Brick> bricks;

    void StartGame(CRect boundary, CWnd* pWnd);
    void DrawGame(CDC* pDC);
    void HandleCollisions();
    void EndGame();
    void ResetGame(const CRect& boundary, CWnd* pWnd);
    bool AreAllBricksBroken();
};
