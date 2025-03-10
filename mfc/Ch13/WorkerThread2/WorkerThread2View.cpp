
// WorkerThread2View.cpp: CWorkerThread2View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WorkerThread2.h"
#endif

#include "WorkerThread2Doc.h"
#include "WorkerThread2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorkerThread2View

IMPLEMENT_DYNCREATE(CWorkerThread2View, CView)

BEGIN_MESSAGE_MAP(CWorkerThread2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CWorkerThread2View 생성/소멸

CWorkerThread2View::CWorkerThread2View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWorkerThread2View::~CWorkerThread2View()
{
}

BOOL CWorkerThread2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWorkerThread2View 그리기

void CWorkerThread2View::OnDraw(CDC* /*pDC*/)
{
	CWorkerThread2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CWorkerThread2View 인쇄

BOOL CWorkerThread2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWorkerThread2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWorkerThread2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWorkerThread2View 진단

#ifdef _DEBUG
void CWorkerThread2View::AssertValid() const
{
	CView::AssertValid();
}

void CWorkerThread2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWorkerThread2Doc* CWorkerThread2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorkerThread2Doc)));
	return (CWorkerThread2Doc*)m_pDocument;
}
#endif //_DEBUG


// CWorkerThread2View 메시지 처리기

#define LOOPCNT (200000)
#define DIVCNT (LOOPCNT/500)

UINT MyDraw(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CClientDC dc(pArg->pWnd);
	int x, y, i;
	CBrush brush1(RGB(255, 0, 0));
	CBrush brush2(RGB(0, 0, 255));

	// 서로 다른 위치에 느린 속도로 막대를 출력한다.
	switch (pArg->pos) {
	case 1:
		dc.SelectObject(&brush1);
		x = 50; y = 50;
		for (i = 0; i < LOOPCNT; i++) {
			int inc = i / DIVCNT;
			dc.Rectangle(x, y, x + inc, y + 20);
		}
		break;
	case 2:
		dc.SelectObject(&brush2);
		x = 50; y = 100;
		for (i = 0; i < LOOPCNT; i++) {
			int inc = i / DIVCNT;
			dc.Rectangle(x, y, x + inc, y + 20);
		}
		break;

	}

	return 0;
}

void CWorkerThread2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 화면을 지운다.
	CClientDC dc(this);
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(&rect);

	// 첫 번째 스레드를 중지 상태로 생성한다.
	arg1.pWnd = this; // 뷰 객체의 주소
	arg1.pos = 1; // 출력 위치(1: 위쪽)
	pThread1 = AfxBeginThread(MyDraw, &arg1,
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	// 두 번째 스레드를 중지 상태로 생성한다.
	arg2.pWnd = this; // 뷰 객체의 주소
	arg2.pos = 2; // 출력 위치(2: 아래쪽)
	pThread2 = AfxBeginThread(MyDraw, &arg2,
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	// 두 번째 스레드의 우선순위 레벨을 변경한다.
	pThread2->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

	// 두 스레드의 실행을 재개한다.
	pThread1->ResumeThread();
	pThread2->ResumeThread();
}
