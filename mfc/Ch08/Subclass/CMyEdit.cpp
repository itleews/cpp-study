// CMyEdit.cpp: 구현 파일
//

#include "pch.h"
#include "Subclass.h"
#include "CMyEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{

}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CMyEdit 메시지 처리기



void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar >= _T('0') && nChar <= _T('9'))
		|| nChar == _T('-') || nChar == _T('\b')) {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
