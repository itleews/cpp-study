
// ReflectionView.cpp: CReflectionView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Reflection.h"
#endif

#include "ReflectionDoc.h"
#include "ReflectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReflectionView

IMPLEMENT_DYNCREATE(CReflectionView, CFormView)

BEGIN_MESSAGE_MAP(CReflectionView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CReflectionView 생성/소멸

CReflectionView::CReflectionView() noexcept
	: CFormView(IDD_REFLECTION_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CReflectionView::~CReflectionView()
{
}

void CReflectionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CReflectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CReflectionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_list.AddString(_T("사과"));
	m_list.AddString(_T("포도"));
	m_list.AddString(_T("참외"));
	m_list.AddString(_T("수박"));
}


// CReflectionView 인쇄

BOOL CReflectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CReflectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CReflectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CReflectionView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CReflectionView 진단

#ifdef _DEBUG
void CReflectionView::AssertValid() const
{
	CFormView::AssertValid();
}

void CReflectionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CReflectionDoc* CReflectionView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReflectionDoc)));
	return (CReflectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CReflectionView 메시지 처리기
