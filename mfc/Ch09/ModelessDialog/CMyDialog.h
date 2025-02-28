#pragma once
#include "afxdialogex.h"

class CModelessDialogView; // 참조할 클래스 선언


// CMyDialog 대화 상자

class CMyDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMyDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CModelessDialogView* m_pView; // 뷰 객체의 주소 저장
	CString m_str;
	int m_font;
	afx_msg void OnBnClickedApply();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
