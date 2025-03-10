#pragma once

// CListenSocket 명령 대상
class CTCPServer2Doc;

class CListenSocket : public CSocket
{
public:
	CListenSocket(CTCPServer2Doc* pDoc);
	virtual ~CListenSocket();
	CTCPServer2Doc* m_pDoc;
	virtual void OnAccept(int nErrorCode);
};


