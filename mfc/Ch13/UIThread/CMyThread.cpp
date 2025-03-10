// CMyThread.cpp: 구현 파일
//

#include "pch.h"
#include "UIThread.h"
#include "CMyThread.h"
#include "CMyWnd.h"

// CMyThread

IMPLEMENT_DYNCREATE(CMyThread, CWinThread)

CMyThread::CMyThread()
{
}

CMyThread::~CMyThread()
{
}

BOOL CMyThread::InitInstance()
{
	// 프레임 윈도우 객체를 동적으로 생성한다.
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CMyWnd);
	CObject* pObject = pRuntimeClass->CreateObject();
	// 객체를 성공적으로 생성했는지 확인한다.
	ASSERT(pObject->IsKindOf(RUNTIME_CLASS(CMyWnd)));
	// 프레임 윈도우 객체의 주소를 저장하고, 윈도우를 화면에 보인다.
	m_pMainWnd = (CWnd*)pObject;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CMyThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyThread, CWinThread)
END_MESSAGE_MAP()


// CMyThread 메시지 처리기
