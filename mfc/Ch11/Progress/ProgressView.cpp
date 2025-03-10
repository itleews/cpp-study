
// ProgressView.cpp: CProgressView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Progress.h"
#endif

#include "ProgressDoc.h"
#include "ProgressView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProgressView

IMPLEMENT_DYNCREATE(CProgressView, CFormView)

BEGIN_MESSAGE_MAP(CProgressView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CProgressView 생성/소멸

CProgressView::CProgressView() noexcept
	: CFormView(IDD_PROGRESS_FORM)
{
	bNowProgress = FALSE;

}

CProgressView::~CProgressView()
{
}

void CProgressView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BOOL CProgressView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CProgressView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CProgressView 인쇄

BOOL CProgressView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CProgressView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CProgressView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CProgressView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CProgressView 진단

#ifdef _DEBUG
void CProgressView::AssertValid() const
{
	CFormView::AssertValid();
}

void CProgressView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CProgressDoc* CProgressView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProgressDoc)));
	return (CProgressDoc*)m_pDocument;
}
#endif //_DEBUG


// CProgressView 메시지 처리기

void CProgressView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (bNowProgress == FALSE) {
		bNowProgress = TRUE;
		m_progress.SetRange(0, 30); // 값의 범위를 설정한다.
		m_progress.SetPos(0); // 현재 위치를 설정한다.
		SetTimer(100, 50, NULL); // ID가 100인 타이머를 생성한다.
	}
}

void CProgressView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100) { // 타이머 ID가 100인 경우만 처리한다.
		if (m_progress.GetPos() < 30)
			m_progress.OffsetPos(1); // 현재 위치를 1씩 증가한다.
		else {
			KillTimer(nIDEvent); // 타이머를 파괴한다.
			bNowProgress = FALSE;
		}
	}
}
