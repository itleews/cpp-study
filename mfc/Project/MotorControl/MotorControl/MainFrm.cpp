
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MotorControl.h"
#include "MotorListView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	//{
	//	TRACE0("뷰 창을 만들지 못했습니다.\n");
	//	return -1;
	//}

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// splitter의 왼쪽 첫 번째 pane (ChildView)에 포커스
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	if (pWnd) pWnd->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 왼쪽 뷰에서 먼저 명령 처리 시도
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	if (pWnd && pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 기본 처리
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// 1행 2열 분할 창 만들기
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	// 왼쪽: 도면 뷰 (ChildView)
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CChildView), CSize(700, 0), pContext))
		return FALSE;

	// 오른쪽: 모터 리스트 뷰 (CFormView 기반)
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMotorListView), CSize(300, 0), pContext))
		return FALSE;

	return TRUE; // ✅ 모든 뷰가 성공적으로 생성되었을 때만 TRUE 반환
}
