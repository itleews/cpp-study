#pragma once

#include <afxtempl.h>

// CMyWnd 프레임

class CMyWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMyWnd)
protected:
	CMyWnd();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyWnd();
	CArray<TCHAR, TCHAR> m_str;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
};


