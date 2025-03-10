
// WebBrowserView.cpp: CWebBrowserView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WebBrowser.h"
#endif

#include "WebBrowserDoc.h"
#include "WebBrowserView.h"
#include "CMyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebBrowserView

IMPLEMENT_DYNCREATE(CWebBrowserView, CHtmlView)

BEGIN_MESSAGE_MAP(CWebBrowserView, CHtmlView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_WEBSEARCH_OPEN, &CWebBrowserView::OnWebsearchOpen)
	ON_COMMAND(ID_WEBSEARCH_BACK, &CWebBrowserView::OnWebsearchBack)
	ON_COMMAND(ID_WEBSEARCH_FORWARD, &CWebBrowserView::OnWebsearchForward)
END_MESSAGE_MAP()

// CWebBrowserView 생성/소멸

CWebBrowserView::CWebBrowserView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWebBrowserView::~CWebBrowserView()
{
}

BOOL CWebBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CHtmlView::PreCreateWindow(cs);
}

void CWebBrowserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	m_pBrowserApp->put_Silent(VARIANT_TRUE); // 스크립트 오류 표시 끄기
	GoHome();
}


// CWebBrowserView 인쇄



// CWebBrowserView 진단

#ifdef _DEBUG
void CWebBrowserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebBrowserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CWebBrowserDoc* CWebBrowserView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebBrowserDoc)));
	return (CWebBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CWebBrowserView 메시지 처리기

void CWebBrowserView::OnWebsearchOpen()
{
	CMyDialog dlg;
	if (dlg.DoModal() == IDOK)
		Navigate(dlg.m_url);
}


void CWebBrowserView::OnWebsearchBack()
{
	GoBack();
}

void CWebBrowserView::OnWebsearchForward()
{
	GoForward();
}
