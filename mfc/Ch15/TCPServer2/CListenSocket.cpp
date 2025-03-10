// CListenSocket.cpp: 구현 파일
//

#include "pch.h"
#include "TCPServer2.h"
#include "CListenSocket.h"
#include "TCPServer2Doc.h"

// CListenSocket

CListenSocket::CListenSocket(CTCPServer2Doc* pDoc)
{
	m_pDoc = pDoc;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 멤버 함수

void CListenSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	m_pDoc->ProcessAccept(nErrorCode);
}
