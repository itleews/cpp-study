
// WorkerThread1View.cpp: CWorkerThread1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WorkerThread1.h"
#endif

#include "WorkerThread1Doc.h"
#include "WorkerThread1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorkerThread1View

IMPLEMENT_DYNCREATE(CWorkerThread1View, CView)

BEGIN_MESSAGE_MAP(CWorkerThread1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CWorkerThread1View 생성/소멸

CWorkerThread1View::CWorkerThread1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWorkerThread1View::~CWorkerThread1View()
{
}

BOOL CWorkerThread1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWorkerThread1View 그리기

void CWorkerThread1View::OnDraw(CDC* /*pDC*/)
{
	CWorkerThread1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CWorkerThread1View 인쇄

BOOL CWorkerThread1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWorkerThread1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWorkerThread1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWorkerThread1View 진단

#ifdef _DEBUG
void CWorkerThread1View::AssertValid() const
{
	CView::AssertValid();
}

void CWorkerThread1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWorkerThread1Doc* CWorkerThread1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorkerThread1Doc)));
	return (CWorkerThread1Doc*)m_pDocument;
}
#endif //_DEBUG

UINT CalcIt(LPVOID arg)
{
	// LPVOID를 int형으로 타입 캐스팅한다.
	int val = (int)arg;

	// 1부터 val까지 1씩 증가하면서 더한다.
	int result = 0;
	for (int i = 1; i < val; i++) {
		result += i;
		Sleep(10); // 테스트를 위해 계산 속도를 늦춘다.
	}

	// 계산 결과를 표시한다.
	CString str;
	str.Format(_T("계산 결과 = %d"), result);
	AfxMessageBox(str);

	return 0;
}


// CWorkerThread1View 메시지 처리기

void CWorkerThread1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	int val = 600;

	/* 스레드를 사용하지 않은 경우 */
	// CalcIt((LPVOID)val);

	/* 스레드를 사용한 경우 */
	AfxBeginThread(CalcIt, (LPVOID)val);
}
