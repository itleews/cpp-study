
// WebBrowserView.h: CWebBrowserView 클래스의 인터페이스
//

#pragma once


class CWebBrowserView : public CHtmlView
{
protected: // serialization에서만 만들어집니다.
	CWebBrowserView() noexcept;
	DECLARE_DYNCREATE(CWebBrowserView)

// 특성입니다.
public:
	CWebBrowserDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CWebBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWebsearchOpen();
	afx_msg void OnWebsearchBack();
	afx_msg void OnWebsearchForward();
};

#ifndef _DEBUG  // WebBrowserView.cpp의 디버그 버전
inline CWebBrowserDoc* CWebBrowserView::GetDocument() const
   { return reinterpret_cast<CWebBrowserDoc*>(m_pDocument); }
#endif

