
// ListViewTestView.cpp: CListViewTestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ListViewTest.h"
#endif

#include "ListViewTestDoc.h"
#include "ListViewTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CListViewTestView

IMPLEMENT_DYNCREATE(CListViewTestView, CListView)

BEGIN_MESSAGE_MAP(CListViewTestView, CListView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
	ON_COMMAND(ID_VIEW_LARGEICON, &CListViewTestView::OnViewLargeicon)
	ON_COMMAND(ID_VIEW_SMALLICON, &CListViewTestView::OnViewSmallicon)
	ON_COMMAND(ID_VIEW_LIST, &CListViewTestView::OnViewList)
	ON_COMMAND(ID_VIEW_DETAILS, &CListViewTestView::OnViewDetails)
END_MESSAGE_MAP()

// CListViewTestView 생성/소멸

CListViewTestView::CListViewTestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CListViewTestView::~CListViewTestView()
{
}

BOOL CListViewTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK; // 네 개의 보기 스타일을 모두 제거한다.
	cs.style |= LVS_REPORT; // 보고서 보기 스타일을 추가한다.
	return CListView::PreCreateWindow(cs);
}


void CListViewTestView::OnDraw(CDC* /*pDC*/)
{
	CListViewTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


void CListViewTestView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// 이미지 리스트 생성과 초기화
	CImageList ilLarge, ilSmall;
	ilLarge.Create(32, 32, ILC_COLOR4, 1, 1);
	ilSmall.Create(16, 16, ILC_COLOR4, 1, 1);
	ilLarge.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	ilSmall.Add(AfxGetApp()->LoadIcon(IDI_ICON1));

	// 이미지 리스트 설정
	CListCtrl& list = GetListCtrl();
	list.SetImageList(&ilLarge, LVSIL_NORMAL);
	list.SetImageList(&ilSmall, LVSIL_SMALL);
	ilLarge.Detach();
	ilSmall.Detach();

	// 헤더 초기화
	list.InsertColumn(0, _T("상품명"), LVCFMT_LEFT, 100, 0);
	list.InsertColumn(1, _T("가격"), LVCFMT_LEFT, 100, 2);
	list.InsertColumn(2, _T("수량"), LVCFMT_LEFT, 100, 1);

	CListViewTestDoc* pDoc = GetDocument();
	CString str;
	for (int i = 0; i < NUMITEM; i++) {
		// 항목 추가
		list.InsertItem(i, pDoc->m_item[i].name, 0);
		// 하위 항목 추가
		str.Format(_T("%d"), pDoc->m_item[i].price);
		list.SetItemText(i, 1, str);
		str.Format(_T("%d"), pDoc->m_item[i].quantity);
		list.SetItemText(i, 2, str);
	}
}


// CListViewTestView 인쇄

BOOL CListViewTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CListViewTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CListViewTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CListViewTestView 진단

#ifdef _DEBUG
void CListViewTestView::AssertValid() const
{
	CListView::AssertValid();
}

void CListViewTestView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CListViewTestDoc* CListViewTestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CListViewTestDoc)));
	return (CListViewTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CListViewTestView 메시지 처리기

void CListViewTestView::OnViewLargeicon()
{
	ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

void CListViewTestView::OnViewSmallicon()
{
	ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

void CListViewTestView::OnViewList()
{
	ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}

void CListViewTestView::OnViewDetails()
{
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}
