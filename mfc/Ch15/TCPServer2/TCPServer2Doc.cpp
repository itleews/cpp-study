
// TCPServer2Doc.cpp: CTCPServer2Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TCPServer2.h"
#endif

#include "TCPServer2Doc.h"
#include "TCPServer2View.h"
#include "CListenSocket.h"
#include "CDataSocket.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTCPServer2Doc

IMPLEMENT_DYNCREATE(CTCPServer2Doc, CDocument)

BEGIN_MESSAGE_MAP(CTCPServer2Doc, CDocument)
END_MESSAGE_MAP()


// CTCPServer2Doc 생성/소멸

CTCPServer2Doc::CTCPServer2Doc() noexcept
{
	m_pListenSocket = NULL;
	m_pDataSocket = NULL;
}

CTCPServer2Doc::~CTCPServer2Doc()
{
	if (m_pListenSocket != NULL)
		delete m_pListenSocket;
	if (m_pDataSocket != NULL)
		delete m_pDataSocket;
}

BOOL CTCPServer2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(nullptr);
	}

	ASSERT(m_pListenSocket == NULL);
	m_pListenSocket = new CListenSocket(this);
	if (m_pListenSocket->Create(8000)) {
		if (m_pListenSocket->Listen()) {
			AfxMessageBox(_T("서버를 시작합니다."), MB_ICONINFORMATION);
			return TRUE;
		}
	}

	AfxMessageBox(_T("이미 실행 중인 서버가 있습니다.")
		_T("\n프로그램을 종료합니다."), MB_ICONERROR);
	return FALSE;
}




// CTCPServer2Doc serialization

void CTCPServer2Doc::Serialize(CArchive& ar)
{
	// CEditView에는 모든 serialization을 처리하는 edit 컨트롤이 들어 있습니다.
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != nullptr)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CTCPServer2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CTCPServer2Doc::InitializeSearchContent()
{
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 전체 텍스트 파일 콘텐츠를 검색 콘텐츠로 사용합니다.
	SetSearchContent(m_strSearchContent);
}

void CTCPServer2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTCPServer2Doc 진단

#ifdef _DEBUG
void CTCPServer2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTCPServer2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTCPServer2Doc 명령

void CTCPServer2Doc::DeleteContents()
{
	if (m_pListenSocket != NULL) {
		delete m_pListenSocket;
		m_pListenSocket = NULL;
	}
	if (m_pDataSocket != NULL) {
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
	CDocument::DeleteContents();
}

void CTCPServer2Doc::PrintMessage(LPCTSTR message)
{
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CTCPServer2View* pView = (CTCPServer2View*)pMainWnd->GetActiveView();
	pView->AddMessage(message);
}

void CTCPServer2Doc::ProcessAccept(int nErrorCode)
{
	CString PeerAddr;
	UINT PeerPort;
	CString str;

	ASSERT(nErrorCode == 0);
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		if (m_pListenSocket->Accept(*m_pDataSocket)) {
			m_pDataSocket->GetPeerName(PeerAddr, PeerPort);
			str.Format(_T("### IP 주소: %s, 포트 번호: %d ###\r\n"),
				PeerAddr, PeerPort);
			PrintMessage(str);
		}
		else {
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
}

void CTCPServer2Doc::ProcessReceive(CDataSocket* pSocket, int nErrorCode)
{
	TCHAR buf[256 + 1];
	int nbytes = pSocket->Receive(buf, 256);
	buf[nbytes] = _T('\0');
	PrintMessage(buf);
}

void CTCPServer2Doc::ProcessClose(CDataSocket* pSocket, int nErrorCode)
{
	pSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;
	PrintMessage(_T("### 접속 종료 ###\r\n\r\n"));
}
