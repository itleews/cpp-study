
// ImageListView.cpp: CImageListView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageList.h"
#endif

#include "ImageListDoc.h"
#include "ImageListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageListView

IMPLEMENT_DYNCREATE(CImageListView, CView)

BEGIN_MESSAGE_MAP(CImageListView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CImageListView 생성/소멸

CImageListView::CImageListView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageListView::~CImageListView()
{
}

BOOL CImageListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageListView 그리기

void CImageListView::OnDraw(CDC* pDC)
{
	CImageListDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 클라이언트 영역을 패턴 브러시로 칠한다.
	CBrush brush(HS_DIAGCROSS, RGB(255, 0, 0));
	pDC->SelectObject(&brush);
	pDC->SelectStockObject(NULL_PEN);
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(&rect);

	// 배경을 투명색으로 설정하고 출력한다.
	m_il.SetBkColor(CLR_NONE);
	m_il.Draw(pDC, 0, CPoint(50, 50), ILD_NORMAL);
	m_il.Draw(pDC, 0, CPoint(150, 50), ILD_TRANSPARENT);
	m_il.Draw(pDC, 0, CPoint(250, 50), ILD_BLEND25);
	m_il.Draw(pDC, 0, CPoint(350, 50), ILD_BLEND50);
	m_il.Draw(pDC, 0, CPoint(450, 50), ILD_MASK);

	// 배경을 검은색으로 설정하고 출력한다.
	m_il.SetBkColor(RGB(0, 0, 0));
	m_il.Draw(pDC, 1, CPoint(50, 150), ILD_NORMAL);
	m_il.Draw(pDC, 1, CPoint(150, 150), ILD_TRANSPARENT);
	m_il.Draw(pDC, 1, CPoint(250, 150), ILD_BLEND25);
	m_il.Draw(pDC, 1, CPoint(350, 150), ILD_BLEND50);
	m_il.Draw(pDC, 1, CPoint(450, 150), ILD_MASK);
}


// CImageListView 인쇄

BOOL CImageListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageListView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageListView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CImageListView 진단

#ifdef _DEBUG
void CImageListView::AssertValid() const
{
	CView::AssertValid();
}

void CImageListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageListDoc* CImageListView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageListDoc)));
	return (CImageListDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageListView 메시지 처리기

int CImageListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_il.Create(IDB_BITMAP1, 48, 1, RGB(255, 255, 255));

	return 0;
}
