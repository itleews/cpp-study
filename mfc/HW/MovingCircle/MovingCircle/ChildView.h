
// ChildView.h: CChildView 클래스의 인터페이스
//

#include <vector>
#pragma once

class CCircle {
public:
	int m_x, m_y;
	int m_radius;
	int m_dx, m_dy;
	COLORREF m_circleColor;

	CCircle() : m_x(0), m_y(0), m_radius(0), m_dx(0), m_dy(0), m_circleColor(RGB(0, 0, 0)) {}

	CCircle (int x, int y, int radius, int dx, int dy) : 
		m_x(x), m_y(y), m_radius(radius), m_dx(dx), m_dy(dy), m_circleColor(RGB(0, 0, 0)) {}

    ~CCircle();

    void CCircle::Move(CRect boundary, CWnd* pWnd) {
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
        if (m_y + m_radius + m_dy >= boundary.bottom) {
            m_dy = -m_dy;
            m_y = boundary.bottom - m_radius;
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

	void SpeedUp(int maxSpeed) {
		if (abs(m_dx) >= maxSpeed) {
			MessageBox(NULL, _T("최고 속도입니다."), _T("알림"), MB_OK | MB_ICONERROR);
			return;
		}
		else if (abs(m_dx) < maxSpeed) {
			m_dx *= 2;
			m_dy *= 2;
		}
	}

    void SpeedDown(int minSpeed) {
		if (abs(m_dx) <= minSpeed) {
			MessageBox(NULL, _T("최저 속도입니다."), _T("알림"), MB_OK | MB_ICONERROR);
			return;
		}
		else if (abs(m_dx) > minSpeed) {
			m_dx /= 2;
			m_dy /= 2;
		}
    }

	void ChangeDirection(int checkX, int checkY) {
		if (checkX == 0 && checkY == 0) {
			MessageBox(NULL, _T("방향을 선택하세요."), _T("알림"), MB_OK | MB_ICONWARNING);
			return;
		}
		if (checkX == BST_CHECKED) {
			m_dx = -m_dx;
		}
		if (checkY == BST_CHECKED) {
			m_dy = -m_dy;
		}
	}
};

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
   CChildView();

// 특성입니다.
public:
   int m_circleCount;
   CSpinButtonCtrl m_spinCtrl;  // 스핀 버튼
   CEdit m_editCtrl;            // 값 표시용 에디트 박스
   CRect m_boundary;
   CButton m_speedUp;
   CButton m_speedDown;
   CButton m_checkX;
   CButton m_checkY;
   CButton m_changeDirection;
   CStatic m_speedDisplay; // 속도 표시
   // void UpdateSpeedDisplay();
   std::vector<CCircle> m_circles;

// 작업입니다.
public:

// 재정의입니다.
protected:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
   virtual ~CChildView();

   // 생성된 메시지 맵 함수
protected:
   afx_msg void OnPaint();
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnDestroy();
   afx_msg void OnChangeDirection();
   afx_msg void OnSpeedUp();
   afx_msg void OnSpeedDown();
   afx_msg void OnEditChanged();
};

