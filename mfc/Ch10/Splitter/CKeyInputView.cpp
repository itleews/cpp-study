// CKeyInputView.cpp: 구현 파일
//

#include "pch.h"
#include "Splitter.h"
#include "SplitterDoc.h"
#include "CKeyInputView.h"


// CKeyInputView

IMPLEMENT_DYNCREATE(CKeyInputView, CView)

CKeyInputView::CKeyInputView()
{

}

CKeyInputView::~CKeyInputView()
{
}

BEGIN_MESSAGE_MAP(CKeyInputView, CView)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CKeyInputView 그리기

void CKeyInputView::OnDraw(CDC* pDC)
{
	CSplitterDoc* pDoc = (CSplitterDoc*)GetDocument();

	// 화면 출력용 폰트를 선택한다.
	CFont font;
	font.CreatePointFont(150, _T("궁서"));
	pDC->SelectObject(&font);

	// 현재까지 입력된 글자들을 화면에 출력한다.
	CRect rect;
	GetClientRect(&rect);
	pDC->DrawText(pDoc->m_str.GetData(),
		pDoc->m_str.GetSize(), &rect, DT_LEFT);
}


// CKeyInputView 진단

#ifdef _DEBUG
void CKeyInputView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CKeyInputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CKeyInputView 메시지 처리기

void CKeyInputView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CSplitterDoc* pDoc = (CSplitterDoc*)GetDocument();

	// [Backspace] 입력 시 맨 마지막 글자를 삭제한다.
	if (nChar == _T('\b')) {
		if (pDoc->m_str.GetSize() > 0) {
			pDoc->m_str.RemoveAt(pDoc->m_str.GetSize() - 1);
		}
	}
	// 그 밖의 경우에는 동적 배열에 글자를 추가한다.
	else {
		pDoc->m_str.Add(nChar);
	}

	// 화면을 갱신한다.
	Invalidate();
}
