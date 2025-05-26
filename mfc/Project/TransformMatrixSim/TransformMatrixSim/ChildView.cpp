
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "TransformMatrixSim.h"
#include "ChildView.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_theta = 0.0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

struct Matrix3x3 {
    double m[3][3];

    Matrix3x3 operator*(const Matrix3x3& other) const {
        Matrix3x3 result{};
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];
        return result;
    }

    POINT transformPoint(double x, double y) const {
        POINT p;
        p.x = static_cast<LONG>(m[0][0] * x + m[0][1] * y + m[0][2]);
        p.y = static_cast<LONG>(m[1][0] * x + m[1][1] * y + m[1][2]);
        return p;
    }
};

Matrix3x3 translation(double x, double y) {
    return Matrix3x3{ {
        {1, 0, x},
        {0, 1, y},
        {0, 0, 1}
    } };
}

Matrix3x3 rotation(double theta) {
    return Matrix3x3{ {
        {cos(theta), -sin(theta), 0},
        {sin(theta),  cos(theta), 0},
        {0, 0, 1}
    } };
}

Matrix3x3 scaling(double sx, double sy) {
    return Matrix3x3{ {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    } };
}

Matrix3x3 makeTransform(double x, double y, double theta, double sx, double sy) {
    return translation(x, y) * rotation(theta) * scaling(sx, sy);
}

// 타이머 메시지 처리기
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        m_theta += 0.05;  // 회전 각도 조금씩 증가 (약 3도씩)
        if (m_theta > 2 * 3.141592)
            m_theta -= 2 * 3.141592;

        Invalidate();  // 화면 다시 그리기 요청
    }

    CWnd::OnTimer(nIDEvent);
}

// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);
	return TRUE;
}

void CChildView::OnPaint()
{
    CPaintDC dc(this);

    double x = 300;  // 중심 원 위치 X
    double y = 300;  // 중심 원 위치 Y
    double sx = 10; // X 방향 스케일링 (1.0 = 원래 크기)
	double sy = 10; // Y 방향 스케일링 (1.0 = 원래 크기)
    double theta = m_theta; // 현재 회전 각도

    Matrix3x3 transform = makeTransform(x, y, theta, sx, sy);

    // 원 그리기
    POINT center = transform.transformPoint(0, 0);
    int rx = static_cast<int>(30 * sx);
    int ry = static_cast<int>(30 * sy);
    dc.Ellipse(center.x - rx, center.y - ry, center.x + rx, center.y + ry);


    // 사각형 크기와 거리
    int rectSize = 20;
    int distance = 60;

    // 사각형 로컬 좌표 (중심 기준)
    struct RectPos { double x, y; };
    RectPos rects[4] = {
        {0, -distance},  // 위
        {0,  distance},  // 아래
        {-distance, 0},  // 왼쪽
        { distance, 0}   // 오른쪽
    };

    for (int i = 0; i < 4; ++i)
    {
        POINT rectCenter = transform.transformPoint(rects[i].x, rects[i].y);

        // 사각형 좌표 (왼쪽 위, 오른쪽 아래)
        CRect rc(
            rectCenter.x - rectSize / 2,
            rectCenter.y - rectSize / 2,
            rectCenter.x + rectSize / 2,
            rectCenter.y + rectSize / 2);

        dc.Rectangle(rc);
    }
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    SetTimer(1, 50, nullptr);  // 50ms마다 타이머 이벤트 발생
    return 0;
}

void CChildView::OnDestroy()
{
    KillTimer(1);  // 타이머 종료
    CWnd::OnDestroy();
}
