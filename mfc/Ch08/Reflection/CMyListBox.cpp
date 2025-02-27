// CMyListBox.cpp: 구현 파일
//

#include "pch.h"
#include "Reflection.h"
#include "CMyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{

}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_CONTROL_REFLECT(LBN_DBLCLK, &CMyListBox::OnLbnDblclk)
END_MESSAGE_MAP()



// CMyListBox 메시지 처리기



void CMyListBox::OnLbnDblclk()
{
	CString str;
	int nIndex = GetCurSel();
	GetText(nIndex, str);
	MessageBox(str);
}
