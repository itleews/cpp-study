
// TreeViewTestView.cpp: CTreeViewTestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TreeViewTest.h"
#endif

#include "TreeViewTestDoc.h"
#include "TreeViewTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTreeViewTestView

IMPLEMENT_DYNCREATE(CTreeViewTestView, CTreeView)

BEGIN_MESSAGE_MAP(CTreeViewTestView, CTreeView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTreeView::OnFilePrintPreview)
	ON_COMMAND(ID_TEST_DELETESELECTED, &CTreeViewTestView::OnTestDeleteselected)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeViewTestView::OnTvnSelchanged)
END_MESSAGE_MAP()

// CTreeViewTestView 생성/소멸

CTreeViewTestView::CTreeViewTestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTreeViewTestView::~CTreeViewTestView()
{
}

BOOL CTreeViewTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASBUTTONS;
	cs.style |= TVS_HASLINES;
	cs.style |= TVS_LINESATROOT;
	cs.style |= TVS_TRACKSELECT;
	return CTreeView::PreCreateWindow(cs);
}


void CTreeViewTestView::OnDraw(CDC* /*pDC*/)
{
	CTreeViewTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


void CTreeViewTestView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// 이미지 리스트 생성과 초기화
	CImageList il;
	il.Create(IDB_BITMAP1, 16, 1, RGB(255, 255, 255));

	// 이미지 리스트 설정
	CTreeCtrl& tree = GetTreeCtrl();
	tree.SetImageList(&il, TVSIL_NORMAL);
	il.Detach();

	// 항목 추가
	/* 1st 레벨 초기화 */
	HTREEITEM hSun = tree.InsertItem(_T("태양"), 0, 0, TVI_ROOT, TVI_LAST);

	/* 2nd 레벨 초기화 */
	HTREEITEM hPlanet[8];
	CString planet[] = {
		_T("수성"), _T("금성"), _T("지구"), _T("화성"), _T("목성"), _T("토성"), _T("천왕성"), _T("해왕성")
	};
	for (int i = 0; i < 8; i++) {
		hPlanet[i] = tree.InsertItem(planet[i], 1, 1, hSun, TVI_LAST);
	}

	/* 3rd 레벨 초기화 */ 
	tree.InsertItem(_T("달"), 2, 2, hPlanet[2], TVI_LAST);
	tree.InsertItem(_T("포보스"), 2, 2, hPlanet[3], TVI_LAST);
	tree.InsertItem(_T("데이모스"), 2, 2, hPlanet[3], TVI_LAST);
	tree.InsertItem(_T("이오"), 2, 2, hPlanet[4], TVI_LAST);
	tree.InsertItem(_T("에우로파"), 2, 2, hPlanet[4], TVI_LAST);
	tree.InsertItem(_T("가니메데"), 2, 2, hPlanet[4], TVI_LAST);
	tree.InsertItem(_T("칼리스토"), 2, 2, hPlanet[4], TVI_LAST);
}


// CTreeViewTestView 인쇄

BOOL CTreeViewTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTreeViewTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTreeViewTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTreeViewTestView 진단

#ifdef _DEBUG
void CTreeViewTestView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTreeViewTestView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CTreeViewTestDoc* CTreeViewTestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreeViewTestDoc)));
	return (CTreeViewTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTreeViewTestView 메시지 처리기

void CTreeViewTestView::OnTestDeleteselected()
{
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hItem = tree.GetSelectedItem();
	if (hItem != NULL) {
		tree.DeleteItem(hItem);
	}
}

void CTreeViewTestView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	TVITEM tvi = pNMTreeView->itemNew;
	CTreeCtrl& tree = GetTreeCtrl();
	CString str = tree.GetItemText(tvi.hItem);
	AfxGetMainWnd()->SetWindowText(str);
	*pResult = 0;
}
