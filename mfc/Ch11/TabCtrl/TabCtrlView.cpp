
// TabCtrlView.cpp: CTabCtrlView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TabCtrl.h"
#endif

#include "TabCtrlDoc.h"
#include "TabCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTabCtrlView

IMPLEMENT_DYNCREATE(CTabCtrlView, CFormView)

BEGIN_MESSAGE_MAP(CTabCtrlView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabCtrlView::OnTcnSelchangeTab1)
END_MESSAGE_MAP()

// CTabCtrlView 생성/소멸

CTabCtrlView::CTabCtrlView() noexcept
	: CFormView(IDD_TABCTRL_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTabCtrlView::~CTabCtrlView()
{
}

void CTabCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BOOL CTabCtrlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CTabCtrlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// 탭 컨트롤에 사용할 이미지 리스트를 설정한다.
	CImageList m_il;
	m_il.Create(IDB_BITMAP1, 16, 1, RGB(255, 255, 255));
	m_tab.SetImageList(&m_il);
	m_il.Detach();

	// 탭을 세 개 추가한다.
	m_tab.InsertItem(0, _T("Tab #0"), 0);
	m_tab.InsertItem(1, _T("Tab #1"), 0);
	m_tab.InsertItem(2, _T("Tab #2"), 0);

	// 두 번째 탭이 선택된 상태로 시작한다.
	m_tab.SetCurSel(1);
	m_edit.SetWindowTextW(_T("Tab #1 선택됨"));
}


// CTabCtrlView 인쇄

BOOL CTabCtrlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTabCtrlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTabCtrlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CTabCtrlView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CTabCtrlView 진단

#ifdef _DEBUG
void CTabCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTabCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTabCtrlDoc* CTabCtrlView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabCtrlDoc)));
	return (CTabCtrlDoc*)m_pDocument;
}
#endif //_DEBUG


// CTabCtrlView 메시지 처리기

void CTabCtrlView::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = m_tab.GetCurSel();
	CString str;
	str.Format(_T("Tab #%d 선택됨"), nIndex);
	m_edit.SetWindowTextW(str);
	*pResult = 0;
}
