
// WorkerThread2View.h: CWorkerThread2View 클래스의 인터페이스
//

#pragma once

struct ThreadArg
{
	CWnd* pWnd; // 출력할 대상 윈도우 객체의 주소이다.
	int pos; // 1이면 위쪽에, 2이면 아래쪽에 출력한다.
};

class CWorkerThread2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CWorkerThread2View() noexcept;
	DECLARE_DYNCREATE(CWorkerThread2View)

// 특성입니다.
public:
	CWorkerThread2Doc* GetDocument() const;
	CWinThread* pThread1, * pThread2; // 스레드 객체 주소
	ThreadArg arg1, arg2; // 스레드 전달 인자

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
	virtual ~CWorkerThread2View();
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

#ifndef _DEBUG  // WorkerThread2View.cpp의 디버그 버전
inline CWorkerThread2Doc* CWorkerThread2View::GetDocument() const
   { return reinterpret_cast<CWorkerThread2Doc*>(m_pDocument); }
#endif

