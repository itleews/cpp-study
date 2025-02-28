// CMyDialog.cpp: 구현 파일
//

#include "pch.h"
#include "ModalDialog3.h"
#include "afxdialogex.h"
#include "CMyDialog.h"


// CMyDialog 대화 상자

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

CMyDialog::CMyDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_str(_T(""))
	, m_font(0)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR, m_str);
	DDV_MaxChars(pDX, m_str, 10);
	DDX_CBIndex(pDX, IDC_FONT, m_font);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_BN_CLICKED(IDC_CLEAR, &CMyDialog::OnBnClickedClear)
END_MESSAGE_MAP()


// CMyDialog 메시지 처리기

void CMyDialog::OnBnClickedClear()
{
	EndDialog(IDC_CLEAR);
}
