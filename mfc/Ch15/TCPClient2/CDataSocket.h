#pragma once

// CDataSocket 명령 대상
class CTCPClient2Dlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CTCPClient2Dlg* pDlg);
	virtual ~CDataSocket();
	CTCPClient2Dlg* m_pDlg;
};


