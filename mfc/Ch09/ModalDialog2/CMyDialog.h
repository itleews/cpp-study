#pragma once
#include "afxdialogex.h"


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
	CComboBox m_combo;
	CString m_str; // 사용자가 입력한 텍스트 저장
	int m_font; // 사용자가 선택한 폰트 번호 저장
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedClear();
	afx_msg void OnEnChangeStr();
};
