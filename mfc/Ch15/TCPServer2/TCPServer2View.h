
// TCPServer2View.h: CTCPServer2View 클래스의 인터페이스
//

#pragma once


class CTCPServer2View : public CEditView
{
protected: // serialization에서만 만들어집니다.
	CTCPServer2View() noexcept;
	DECLARE_DYNCREATE(CTCPServer2View)

// 특성입니다.
public:
	CTCPServer2Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTCPServer2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	void AddMessage(LPCTSTR message);
};

#ifndef _DEBUG  // TCPServer2View.cpp의 디버그 버전
inline CTCPServer2Doc* CTCPServer2View::GetDocument() const
   { return reinterpret_cast<CTCPServer2Doc*>(m_pDocument); }
#endif

