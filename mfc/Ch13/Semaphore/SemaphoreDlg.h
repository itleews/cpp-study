
// SemaphoreDlg.h: 헤더 파일
//

#pragma once


// CSemaphoreDlg 대화 상자
class CSemaphoreDlg : public CDialogEx
{
// 생성입니다.
public:
	CSemaphoreDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEMAPHORE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress1;
	CProgressCtrl m_progress2;
	CProgressCtrl m_progress3;
	CProgressCtrl m_progress4;
	CProgressCtrl m_progress5;
	afx_msg void OnBnClickedStart();
};
