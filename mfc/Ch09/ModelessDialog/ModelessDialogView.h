
// ModelessDialogView.h: CModelessDialogView 클래스의 인터페이스
//

#pragma once

class CMyDialog; // 참조할 클래스 선언

class CModelessDialogView : public CView
{
protected: // serialization에서만 만들어집니다.
	CModelessDialogView() noexcept;
	DECLARE_DYNCREATE(CModelessDialogView)

// 특성입니다.
public:
	CModelessDialogDoc* GetDocument() const;
	CMyDialog* m_pDlg; // 대화 상자 객체의 주소 저장
	CString m_str;
	int m_font;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CModelessDialogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ModelessDialogView.cpp의 디버그 버전
inline CModelessDialogDoc* CModelessDialogView::GetDocument() const
   { return reinterpret_cast<CModelessDialogDoc*>(m_pDocument); }
#endif

