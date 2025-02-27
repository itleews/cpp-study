
// ScrollBarView.h: CScrollBarView 클래스의 인터페이스
//

#pragma once


class CScrollBarView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CScrollBarView() noexcept;
	DECLARE_DYNCREATE(CScrollBarView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SCROLLBAR_FORM };
#endif

// 특성입니다.
public:
	CScrollBarDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CScrollBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_status;
	CScrollBar m_vsb;
	CScrollBar m_hsb;
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnDraw(CDC* /*pDC*/);
};

#ifndef _DEBUG  // ScrollBarView.cpp의 디버그 버전
inline CScrollBarDoc* CScrollBarView::GetDocument() const
   { return reinterpret_cast<CScrollBarDoc*>(m_pDocument); }
#endif

