﻿#include "pch.h"  
#include "GameManager.h"  
#include <afxwin.h> // CWnd 포함  
#include <atltypes.h> // CRect 포함  
#include "ChildView.h"
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()
#include <print>

void GameManager::StartGame(CRect boundary, CWnd* pWnd) {
    ballCount = 1;
    
    // 게임 영역의 중앙 계산
    int centerX = boundary.left + (boundary.Width() / 2);

    // 패들 배치 (화면 중앙 아래에 배치)
    int paddleWidth = 200;
    int paddleHeight = 20;
    int paddleX = centerX - paddleWidth / 2;
    int paddleY = boundary.bottom - paddleHeight - 10;

    paddles.push_back(Paddle(paddleX, paddleY, paddleWidth, paddleHeight, 0, 0));

    int ballRadius = 20;

	for (int i = 0; i < ballCount; i++) {
		// 공을 패들 위에 배치
		int ballX = centerX + i * ballRadius;  // 공 간격 조정
		int ballY = paddleY - (i * (ballRadius * 2 + 5));  // 패들의 위쪽에 공 배치
		// 공 생성 및 추가
		balls.push_back(Ball(i, ballX, ballY, ballRadius, 10, -10));  // 속도 (10, -10)
	}

    int rows = 10;
    int cols = 20;
    int brickWidth = (boundary.Width() - 110) / cols;
    int brickHeight = (boundary.Height() / 3) / rows;
    int startX = 20; // 시작 x 좌표
    int startY = 20; // 시작 y 좌표
    int gap = 5; // 벽돌 간의 간격
    m_brickCount = rows * cols;

    srand(time(0));  // 랜덤 시드 설정 (한 번만 호출)  

    for (int row = 0; row < rows; ++row) {  
        for (int col = 0; col < cols; ++col) {  
            int x = startX + col * (brickWidth + gap);  
            int y = startY + row * (brickHeight + gap);

            // 랜덤 내구도 (1~3)  
            int hitCount = (rand() % 3) + 1;
            int r = 0, g = 0, b = 0;
            switch (hitCount) {
            case 1:
                r = 255; g = 99; b = 71;
                break;
            case 2:
                r = 255; g = 0; b = 0;
                break;
            case 3:
                r = 178; g = 34; b = 34;
                break;
            }
            COLORREF color = RGB(r, g, b);

            bricks.push_back(Brick(x, y, brickWidth, brickHeight, color, hitCount));
        }
    }
}

void GameManager::EndGame(CWnd* pWnd) {  
    // 게임 종료  
    KillTimer(pWnd->GetSafeHwnd(), 1);
}  

void GameManager::ResetGame(const CRect& boundary, CWnd* pWnd) {
    // 게임 상태 초기화
    balls.clear();
    paddles.clear();
    bricks.clear();
    StartGame(boundary, pWnd);
    CChildView* pChildView = static_cast<CChildView*>(pWnd);
    pChildView->m_startTick = GetTickCount64();
}

void GameManager::ClearGame(CWnd* pWnd)
{
    EndGame(pWnd);
    MessageBox(pWnd->GetSafeHwnd(), _T("축하합니다! 벽돌이 모두 깨졌습니다!"), _T("승리!"), MB_ICONASTERISK);
    if (AfxMessageBox(_T("게임을 다시 시작하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        CChildView* pChildView = static_cast<CChildView*>(pWnd);
        ResetGame(pChildView->m_boundary, pWnd);
        SetTimer(pWnd->GetSafeHwnd(), 1, 16, nullptr);
    }
    else
    {
        PostQuitMessage(0); // 프로그램 종료  
    }
}

void GameManager::DrawGame(CDC* pDC) {  
    // 게임 오브젝트 그리기
    for (auto& ball : balls) {  
        ball.Draw(pDC);  
    }  
    for (auto& paddle : paddles) {  
        paddle.Draw(pDC);  
    }  
    for (auto& brick : bricks) {  
        if (!brick.isBroken) {  
            brick.Draw(pDC);  
        }  
    }  
}

void GameManager::DestroyBall(Ball* ball) {
    auto it = std::remove_if(balls.begin(), balls.end(), [ball](const Ball& b) {
        return b == *ball; // 값 비교
        });
    balls.erase(it, balls.end());
}

void GameManager::HandleCollisions(CWnd* pWnd) {
    CChildView* pChildView = static_cast<CChildView*>(pWnd);
    CRect boundary = pChildView->m_boundary;

    // 공과 벽돌, 공과 패들 충돌 체크
    
    for (auto& brick : bricks) 
    {
        for (auto& ball : balls)
        {
            if (brick.isBroken) // 부서진 경우
                continue;

            //if (ball.m_x + ball.m_radius < brick.x) // 볼 좌측
            //    continue;

            //if (ball.m_x - ball.m_radius > brick.x + brick.width) // 볼 우측
            //    continue;

            //if (ball.m_y + ball.m_radius < brick.y) // 볼 하단
            //    continue;

            //if (ball.m_y - ball.m_radius > brick.y + brick.height) // 볼 상단
            //    continue;

			double closestX = clamp(ball.m_x, (double)brick.x, (double)(brick.x + brick.width));
			double closestY = clamp(ball.m_y, (double)brick.y, (double)(brick.y + brick.height));

			double dx = ball.m_x - closestX;
			double dy = ball.m_y - closestY;

			double distanceSquared = dx * dx + dy * dy;

            if (distanceSquared > ball.m_radius * ball.m_radius) // 공과 벽돌의 거리
                continue;

            // 공과 벽돌 충돌
            std::println("충돌! 볼 위치 {}, {} / 브릭 위치 [{}, {} / {}, {}]", ball.m_x, ball.m_y, brick.x, brick.y, brick.x + brick.width, brick.y + brick.height);
            

            // std::cout << "충돌한 벽 번호 :  위치 : 

            brick.Break();
            brick.Update(boundary, pWnd);
            if (brick.isBroken)
                --m_brickCount;

            // 방향성 변경
            if (ball.m_dy < 0) {
                std::println("볼 방향성 변경");
                ball.m_dy = -ball.m_dy;
            }

            //for (int i = 0; i < 5; i++) 
            //{
            //    ball.m_dy -= i * 2;
            //}


            if (m_brickCount == 0)
            {
				ClearGame(pWnd);
            }
        }
    }  

    for (auto& paddle : paddles) 
    {
        for (auto& ball : balls)
        {
            if (ball.m_x + ball.m_radius >= paddle.x &&
                ball.m_x - ball.m_radius <= paddle.x + paddle.width &&
                ball.m_y + ball.m_radius >= paddle.y &&
                ball.m_y - ball.m_radius <= paddle.y + paddle.height)
            {

                // 공이 패들의 어느 방향에서 왔는지 판단하여 반사  
                float overlapLeft = (ball.m_x + ball.m_radius) - paddle.x;
                float overlapRight = (paddle.x + paddle.width) - (ball.m_x - ball.m_radius);
                float overlapTop = (ball.m_y + ball.m_radius) - paddle.y;
                float overlapBottom = (paddle.y + paddle.height) - (ball.m_y - ball.m_radius);

                if (min(overlapLeft, overlapRight) < min(overlapTop, overlapBottom)) {
                    ball.m_dx = -ball.m_dx; // 좌우 반사
                    ball.m_dx = 30;
                }
                else {
                    ball.m_dy = -ball.m_dy; // 상하 반사
                    ball.m_dy = 30;
                }

                // 패들의 위치에 따라 공의 속도를 조정 (더 자연스럽게)  
                float hitPosition = (ball.m_x - (paddle.x + paddle.width / 2)) / (paddle.width / 2);
                ball.m_dx += hitPosition * 2.0f;  // 공의 x 속도를 패들에 맞춰 변화  

                // 패들이 위로 밀 때 공이 위로 튕기도록 설정  
                if (ball.m_dy > 0) {
                    ball.m_dy = -ball.m_dy;
                }
            }
        }
    }
}
