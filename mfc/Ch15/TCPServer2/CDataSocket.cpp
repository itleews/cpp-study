// CDataSocket.cpp: 구현 파일
//

#include "pch.h"
#include "TCPServer2.h"
#include "CDataSocket.h"
#include "TCPServer2Doc.h"

// CDataSocket

CDataSocket::CDataSocket(CTCPServer2Doc* pDoc)
{
	m_pDoc = pDoc;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket 멤버 함수

void CDataSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	m_pDoc->ProcessReceive(this, nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	m_pDoc->ProcessClose(this, nErrorCode);
}
