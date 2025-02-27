
// SubclassView.cpp: CSubclassView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Subclass.h"
#endif

#include "SubclassDoc.h"
#include "SubclassView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSubclassView

IMPLEMENT_DYNCREATE(CSubclassView, CFormView)

BEGIN_MESSAGE_MAP(CSubclassView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSubclassView 생성/소멸

CSubclassView::CSubclassView() noexcept
	: CFormView(IDD_SUBCLASS_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSubclassView::~CSubclassView()
{
}

void CSubclassView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
}

BOOL CSubclassView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CSubclassView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_edit2.SubclassDlgItem(IDC_EDIT2, this);
}


// CSubclassView 인쇄

BOOL CSubclassView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSubclassView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSubclassView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CSubclassView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CSubclassView 진단

#ifdef _DEBUG
void CSubclassView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSubclassView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSubclassDoc* CSubclassView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSubclassDoc)));
	return (CSubclassDoc*)m_pDocument;
}
#endif //_DEBUG


// CSubclassView 메시지 처리기
