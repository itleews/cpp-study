
// HtmlViewTestView.h: CHtmlViewTestView 클래스의 인터페이스
//

#pragma once


class CHtmlViewTestView : public CHtmlView
{
protected: // serialization에서만 만들어집니다.
	CHtmlViewTestView() noexcept;
	DECLARE_DYNCREATE(CHtmlViewTestView)

// 특성입니다.
public:
	CHtmlViewTestDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CHtmlViewTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HtmlViewTestView.cpp의 디버그 버전
inline CHtmlViewTestDoc* CHtmlViewTestView::GetDocument() const
   { return reinterpret_cast<CHtmlViewTestDoc*>(m_pDocument); }
#endif

