// CCircleDoc.cpp: 구현 파일
//

#include "pch.h"
#include "MDITest.h"
#include "CCircleDoc.h"


// CCircleDoc

IMPLEMENT_DYNCREATE(CCircleDoc, CDocument)

CCircleDoc::CCircleDoc()
{
}

BOOL CCircleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_points.RemoveAll();

	return TRUE;
}

CCircleDoc::~CCircleDoc()
{
}


BEGIN_MESSAGE_MAP(CCircleDoc, CDocument)
END_MESSAGE_MAP()


// CCircleDoc 진단

#ifdef _DEBUG
void CCircleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCircleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CCircleDoc serialization

void CCircleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}
#endif


// CCircleDoc 명령
