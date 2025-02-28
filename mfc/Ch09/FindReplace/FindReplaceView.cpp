
// FindReplaceView.cpp: CFindReplaceView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FindReplace.h"
#endif

#include "FindReplaceDoc.h"
#include "FindReplaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindReplaceView

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

IMPLEMENT_DYNCREATE(CFindReplaceView, CView)

BEGIN_MESSAGE_MAP(CFindReplaceView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplaceCmd)
END_MESSAGE_MAP()

// CFindReplaceView 생성/소멸

CFindReplaceView::CFindReplaceView() noexcept
{
	pDlg = NULL;
}

CFindReplaceView::~CFindReplaceView()
{
}

BOOL CFindReplaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CFindReplaceView 그리기

void CFindReplaceView::OnDraw(CDC* /*pDC*/)
{
	CFindReplaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CFindReplaceView 인쇄

BOOL CFindReplaceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CFindReplaceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CFindReplaceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CFindReplaceView 진단

#ifdef _DEBUG
void CFindReplaceView::AssertValid() const
{
	CView::AssertValid();
}

void CFindReplaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFindReplaceDoc* CFindReplaceView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFindReplaceDoc)));
	return (CFindReplaceDoc*)m_pDocument;
}
#endif //_DEBUG


// CFindReplaceView 메시지 처리기

void CFindReplaceView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pDlg != NULL) {
		pDlg->SetFocus();
	}
	else {
		pDlg = new CFindReplaceDialog();
		pDlg->Create(FALSE, _T(""), _T(""), FR_DOWN, this);
	}
}

LRESULT CFindReplaceView::OnFindReplaceCmd(WPARAM wParam, LPARAM lParam)
{
	CString str;

	if (pDlg->IsTerminating()) {
		pDlg = NULL;
		return 0;
	}
	else if (pDlg->FindNext()) {
		str.Format(_T("찾을 문자열: %s"), pDlg->GetFindString());
		MessageBox(str);
	}
	else if (pDlg->ReplaceCurrent()) {
		str.Format(_T("찾을 문자열: %s\n\r바꿀 문자열: %s"),
			pDlg->GetFindString(), pDlg->GetReplaceString());
		MessageBox(str);
	}
	else if (pDlg->ReplaceAll()) {
		str.Format(_T("찾을 문자열: %s\n\r바꿀 문자열: %s"),
			pDlg->GetFindString(), pDlg->GetReplaceString());
		MessageBox(str);
	}

	return 0;
}
