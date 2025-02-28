
// MDITestView.cpp: CMDITestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MDITest.h"
#endif

#include "MDITestDoc.h"
#include "MDITestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDITestView

IMPLEMENT_DYNCREATE(CMDITestView, CView)

BEGIN_MESSAGE_MAP(CMDITestView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CMDITestView 생성/소멸

CMDITestView::CMDITestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMDITestView::~CMDITestView()
{
}

BOOL CMDITestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMDITestView 그리기

void CMDITestView::OnDraw(CDC* pDC)
{
	CMDITestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// 화면 출력용 폰트를 선택한다.
	CFont font;
	font.CreateFont(30, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, _T("궁서"));
	pDC->SelectObject(&font);

	// 현재까지 입력된 글자를 화면에 출력한다.
	CRect rect;
	GetClientRect(&rect);
	pDC->DrawText(pDoc->m_str.GetData(),
		pDoc->m_str.GetSize(), &rect, DT_LEFT);
}


// CMDITestView 인쇄

BOOL CMDITestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMDITestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMDITestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMDITestView 진단

#ifdef _DEBUG
void CMDITestView::AssertValid() const
{
	CView::AssertValid();
}

void CMDITestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDITestDoc* CMDITestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDITestDoc)));
	return (CMDITestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMDITestView 메시지 처리기

void CMDITestView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMDITestDoc* pDoc = GetDocument();

	// [Backspace] 입력 시 맨 마지막 글자를 삭제한다.
	if (nChar == _T('\b')) {
		if (pDoc->m_str.GetSize() > 0) {
			pDoc->m_str.RemoveAt(pDoc->m_str.GetSize() - 1);
		}
	}
	// 그 밖의 경우에는 동적 배열에 글자를 추가한다.
	else {
		pDoc->m_str.Add(nChar);
	}

	// 데이터가 수정되었음을 알린다.
	pDoc->SetModifiedFlag();

	// 화면을 갱신한다.
	pDoc->UpdateAllViews(NULL);
}
