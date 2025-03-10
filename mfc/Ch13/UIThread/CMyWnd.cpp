// CMyWnd.cpp: 구현 파일
//

#include "pch.h"
#include "UIThread.h"
#include "CMyWnd.h"


// CMyWnd

IMPLEMENT_DYNCREATE(CMyWnd, CFrameWnd)

CMyWnd::CMyWnd()
{
	Create(NULL, _T("UI 스레드로 생성한 윈도우"));
}

CMyWnd::~CMyWnd()
{
}


BEGIN_MESSAGE_MAP(CMyWnd, CFrameWnd)
	ON_WM_CHAR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMyWnd 메시지 처리기

void CMyWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Backspace] 입력 시 맨 마지막 글자를 삭제한다.
	if (nChar == _T('\b')) {
		if (m_str.GetSize() > 0)
			m_str.RemoveAt(m_str.GetSize() - 1);
	}
	// 그 밖의 경우에는 동적 배열에 글자를 추가한다.
	else {
		m_str.Add(nChar);
	}

	// 화면을 갱신한다.
	Invalidate();
}

void CMyWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// 화면 출력용 폰트를 선택한다.
	CFont font;
	font.CreatePointFont(150, _T("궁서"));
	dc.SelectObject(&font);

	// 현재까지 입력된 글자를 화면에 출력한다.
	CRect rect;
	GetClientRect(&rect);
	dc.DrawText(m_str.GetData(), m_str.GetSize(), &rect, DT_LEFT);
}
