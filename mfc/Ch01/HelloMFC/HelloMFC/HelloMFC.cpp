#include <afxwin.h>

// ���� ���α׷� Ŭ������ �����Ѵ�.
class CHelloApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

// ���� ������ Ŭ������ �����Ѵ�.
class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

// ���� ���α׷� ��ü�� �����Ѵ�.
CHelloApp theApp;

// ���� ���α׷� Ŭ������ �����Ѵ�.
BOOL CHelloApp::InitInstance()
{
	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	return TRUE;
}

// ���� ������ Ŭ������ �����Ѵ�.
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
	// MessageBox(_T("���콺 Ŭ��!"), _T("���콺 �޽���"));

	/* 3.1 AfxMessageBox() �Լ� */
	// AfxMessageBox(_T("���콺 Ŭ��!"));

	/* 3.2 AfxGetApp(), AfxGetMainWnd(), AfxGetAppName() �Լ� */
	TRACE(_T("���� ���α׷� ��ü �ּ�: %p = %p\n"), AfxGetApp(), &theApp);
	TRACE(_T("���� ������ ��ü �ּ�: %p = %p\n"), AfxGetMainWnd(), this);
	TRACE(_T("���� ���α׷� �̸�: %s\n"), AfxGetAppName());

	/* 3.3 AfxGetInstanceHandle() �Լ� */
	// �ν��Ͻ� �ڵ鰪�� ���� ������ �ε�� ���� �޸� �ּҴ�.
	TRACE(_T("�ν��Ͻ� �ڵ�: %p\n"), AfxGetInstanceHandle());
}

// �޽��� ���� �����Ѵ�.
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()