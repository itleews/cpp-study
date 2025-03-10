
// OptimizeScrollView.h: COptimizeScrollView 클래스의 인터페이스
//

#pragma once


class COptimizeScrollView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	COptimizeScrollView() noexcept;
	DECLARE_DYNCREATE(COptimizeScrollView)

// 특성입니다.
public:
	COptimizeScrollDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~COptimizeScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void MyDraw(CDC* pDC, int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // OptimizeScrollView.cpp의 디버그 버전
inline COptimizeScrollDoc* COptimizeScrollView::GetDocument() const
   { return reinterpret_cast<COptimizeScrollDoc*>(m_pDocument); }
#endif

