
// ModelessDialogView.cpp: CModelessDialogView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ModelessDialog.h"
#endif

#include "ModelessDialogDoc.h"
#include "ModelessDialogView.h"
#include "CMyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelessDialogView

IMPLEMENT_DYNCREATE(CModelessDialogView, CView)

BEGIN_MESSAGE_MAP(CModelessDialogView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CModelessDialogView 생성/소멸

CModelessDialogView::CModelessDialogView() noexcept
{
	m_pDlg = NULL;
	m_str = _T("");
	m_font = 0;

}

CModelessDialogView::~CModelessDialogView()
{
}

BOOL CModelessDialogView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CModelessDialogView 그리기

void CModelessDialogView::OnDraw(CDC* pDC)
{
	CModelessDialogDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CFont font;
	CString fontname;

	switch (m_font) {
	case 0:
		fontname = _T("굴림");
		break;
	case 1:
		fontname = _T("궁서");
		break;
	case 2:
		fontname = _T("바탕");
		break;
	}
	font.CreatePointFont(400, fontname);

	pDC->SelectObject(&font);
	pDC->TextOut(10, 10, m_str);
}


// CModelessDialogView 인쇄

BOOL CModelessDialogView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CModelessDialogView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CModelessDialogView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CModelessDialogView 진단

#ifdef _DEBUG
void CModelessDialogView::AssertValid() const
{
	CView::AssertValid();
}

void CModelessDialogView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelessDialogDoc* CModelessDialogView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelessDialogDoc)));
	return (CModelessDialogDoc*)m_pDocument;
}
#endif //_DEBUG


// CModelessDialogView 메시지 처리기

void CModelessDialogView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pDlg != NULL) {
		m_pDlg->SetFocus(); // 키보드 포커스를 대화 상자로 옮긴다.
	}
	else {
		m_pDlg = new CMyDialog(); // 대화 상자 객체를 생성한다.
		m_pDlg->m_pView = this; // 뷰 객체의 주소를 넘겨준다.
		m_pDlg->m_str = m_str; // IDC_STR 컨트롤의 초기 값
		m_pDlg->m_font = m_font; // IDC_FONT 컨트롤의 초기 값
		m_pDlg->Create(IDD_DIALOG1); // 대화 상자를 생성한다.
		m_pDlg->ShowWindow(SW_SHOW); // 대화 상자를 화면에 출력한다.
	}
}
