﻿
// SpinButtonView.cpp: CSpinButtonView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SpinButton.h"
#endif

#include "SpinButtonDoc.h"
#include "SpinButtonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpinButtonView

IMPLEMENT_DYNCREATE(CSpinButtonView, CFormView)

BEGIN_MESSAGE_MAP(CSpinButtonView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CSpinButtonView 생성/소멸

CSpinButtonView::CSpinButtonView() noexcept
	: CFormView(IDD_SPINBUTTON_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSpinButtonView::~CSpinButtonView()
{
}

void CSpinButtonView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
}

BOOL CSpinButtonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CSpinButtonView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_spin.SetRange(0, 100);
	m_spin.SetPos(0);
}


// CSpinButtonView 인쇄

BOOL CSpinButtonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSpinButtonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSpinButtonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CSpinButtonView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CSpinButtonView 진단

#ifdef _DEBUG
void CSpinButtonView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSpinButtonView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSpinButtonDoc* CSpinButtonView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpinButtonDoc)));
	return (CSpinButtonDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpinButtonView 메시지 처리기

void CSpinButtonView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 스핀 버튼 컨트롤에서 발생한 메시지인지 확인한다.
	if (pScrollBar->GetSafeHwnd() == m_spin.GetSafeHwnd()) {
		CString str;
		str.Format(_T("%d"), nPos);
		AfxGetMainWnd()->SetWindowText(str);
		return;
	}

	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}
