
// ScrollViewTestView.cpp: CScrollViewTestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ScrollViewTest.h"
#endif

#include "ScrollViewTestDoc.h"
#include "ScrollViewTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScrollViewTestView

IMPLEMENT_DYNCREATE(CScrollViewTestView, CScrollView)

BEGIN_MESSAGE_MAP(CScrollViewTestView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CScrollViewTestView 생성/소멸

CScrollViewTestView::CScrollViewTestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CScrollViewTestView::~CScrollViewTestView()
{
}

BOOL CScrollViewTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CScrollViewTestView 그리기

void CScrollViewTestView::OnDraw(CDC* pDC)
{
	CScrollViewTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	pDC->TextOutW(20, 20, CString(_T("스크롤 뷰를 연습합니다.")));
	pDC->Ellipse(1930, 1930, 1980, 1980);
}

void CScrollViewTestView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 2000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CScrollViewTestView 인쇄

BOOL CScrollViewTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CScrollViewTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CScrollViewTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CScrollViewTestView 진단

#ifdef _DEBUG
void CScrollViewTestView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScrollViewTestView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CScrollViewTestDoc* CScrollViewTestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScrollViewTestDoc)));
	return (CScrollViewTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CScrollViewTestView 메시지 처리기
