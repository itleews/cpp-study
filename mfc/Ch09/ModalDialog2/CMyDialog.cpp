// CMyDialog.cpp: 구현 파일
//

#include "pch.h"
#include "ModalDialog2.h"
#include "afxdialogex.h"
#include "CMyDialog.h"


// CMyDialog 대화 상자

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

CMyDialog::CMyDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FONT, m_combo);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_BN_CLICKED(IDC_CLEAR, &CMyDialog::OnBnClickedClear)
	ON_EN_CHANGE(IDC_STR, &CMyDialog::OnEnChangeStr)
END_MESSAGE_MAP()


// CMyDialog 메시지 처리기

BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_STR, m_str);
	m_combo.SetCurSel(m_font);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMyDialog::OnOK()
{
	GetDlgItemText(IDC_STR, m_str);
	m_font = m_combo.GetCurSel();
	CDialog::OnOK();
}

void CMyDialog::OnBnClickedClear()
{
	EndDialog(IDC_CLEAR);
}

void CMyDialog::OnEnChangeStr()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
