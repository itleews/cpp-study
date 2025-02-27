
// ReflectionView.h: CReflectionView 클래스의 인터페이스
//

#pragma once
#include "CMyListBox.h"

class CReflectionView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CReflectionView() noexcept;
	DECLARE_DYNCREATE(CReflectionView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_REFLECTION_FORM };
#endif

// 특성입니다.
public:
	CReflectionDoc* GetDocument() const;

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
	virtual ~CReflectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	CMyListBox m_list;
};

#ifndef _DEBUG  // ReflectionView.cpp의 디버그 버전
inline CReflectionDoc* CReflectionView::GetDocument() const
   { return reinterpret_cast<CReflectionDoc*>(m_pDocument); }
#endif

