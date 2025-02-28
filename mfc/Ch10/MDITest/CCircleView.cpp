// CCircleView.cpp: 구현 파일
//

#include "pch.h"
#include "MDITest.h"
#include "CCircleDoc.h"
#include "CCircleView.h"


// CCircleView

IMPLEMENT_DYNCREATE(CCircleView, CView)

CCircleView::CCircleView()
{

}

CCircleView::~CCircleView()
{
}

BEGIN_MESSAGE_MAP(CCircleView, CView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCircleView 그리기

void CCircleView::OnDraw(CDC* pDC)
{
	CCircleDoc* pDoc = (CCircleDoc*)GetDocument();
	pDC->SelectStockObject(LTGRAY_BRUSH);
	for (int i = 0; i < pDoc->m_points.GetSize(); i++) {
		pDC->Ellipse(pDoc->m_points[i].x - 20, pDoc->m_points[i].y - 20,
			pDoc->m_points[i].x + 20, pDoc->m_points[i].y + 20);
	}
}


// CCircleView 진단

#ifdef _DEBUG
void CCircleView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCircleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCircleView 메시지 처리기

void CCircleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCircleDoc* pDoc = (CCircleDoc*)GetDocument();
	pDoc->m_points.Add(point);
	pDoc->UpdateAllViews(NULL);
}
