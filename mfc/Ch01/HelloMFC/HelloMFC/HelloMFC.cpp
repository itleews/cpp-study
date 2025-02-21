#include <afxwin.h>

// 응용 프로그램 클래스를 선언한다.
class CHelloApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

// 메인 윈도우 클래스를 선언한다.
class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

// 응용 프로그램 객체를 선언한다.
CHelloApp theApp;

// 응용 프로그램 클래스를 정의한다.
BOOL CHelloApp::InitInstance()
{
	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

// 메인 윈도우 클래스를 정의한다.
CMainFrame::CMainFrame()
{
	Create(NULL, _T("HelloMFC"));
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this);
	const TCHAR* msg = _T("Hello, MFC");
	dc.TextOutW(100, 100, msg, lstrlen(msg));
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// MessageBox(_T("마우스 클릭!"), _T("마우스 메시지"));

	/* 3.1 AfxMessageBox() 함수 */
	// AfxMessageBox(_T("마우스 클릭!"));

	/* 3.2 AfxGetApp(), AfxGetMainWnd(), AfxGetAppName() 함수 */
	TRACE(_T("응용 프로그램 객체 주소: %p = %p\n"), AfxGetApp(), &theApp);
	TRACE(_T("메인 윈도우 객체 주소: %p = %p\n"), AfxGetMainWnd(), this);
	TRACE(_T("응용 프로그램 이름: %s\n"), AfxGetAppName());

	/* 3.3 AfxGetInstanceHandle() 함수 */
	// 인스턴스 핸들값은 실행 파일이 로드된 가상 메모리 주소다.
	TRACE(_T("인스턴스 핸들: %p\n"), AfxGetInstanceHandle());
}

// 메시지 맵을 선언한다.
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()