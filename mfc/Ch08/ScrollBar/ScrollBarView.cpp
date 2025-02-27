
// ScrollBarView.cpp: CScrollBarView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ScrollBar.h"
#endif

#include "ScrollBarDoc.h"
#include "ScrollBarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScrollBarView

IMPLEMENT_DYNCREATE(CScrollBarView, CFormView)

BEGIN_MESSAGE_MAP(CScrollBarView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CScrollBarView::OnNMThemeChangedScrollbar1)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &CScrollBarView::OnNMThemeChangedScrollbar2)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CScrollBarView 생성/소멸

CScrollBarView::CScrollBarView() noexcept
	: CFormView(IDD_SCROLLBAR_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CScrollBarView::~CScrollBarView()
{
}

void CScrollBarView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vsb);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_hsb);
}

BOOL CScrollBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CScrollBarView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_hsb.SetScrollRange(0, 360, FALSE); // 0~360 범위 설정
	m_hsb.SetScrollPos(0);
	m_vsb.SetScrollRange(0, 255, FALSE); // 0~255 범위 설정
	m_vsb.SetScrollPos(128);
}


// CScrollBarView 인쇄

BOOL CScrollBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CScrollBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CScrollBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CScrollBarView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CScrollBarView 진단

#ifdef _DEBUG
void CScrollBarView::AssertValid() const
{
	CFormView::AssertValid();
}

void CScrollBarView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CScrollBarDoc* CScrollBarView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScrollBarDoc)));
	return (CScrollBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CScrollBarView 메시지 처리기

void CScrollBarView::OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CScrollBarView::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CScrollBarView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL) { // 스크롤바 컨트롤에서 발생한 메시지이면...
		if (pScrollBar->GetSafeHwnd() == m_hsb.GetSafeHwnd()) {
			switch (nSBCode) {
			case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				break;
			}
			Invalidate();
		}
	}
	else { // 윈도우 스크롤바에서 발생한 메시지이면...
		CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


void CScrollBarView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos;
	int delta = 0;
	if (pScrollBar != NULL) { // 스크롤바 컨트롤에서 발생한 메시지이면...
		if (pScrollBar->GetSafeHwnd() == m_vsb.GetSafeHwnd()) {
			switch (nSBCode) {
			case SB_THUMBTRACK:
				pScrollBar->SetScrollPos(nPos);
				break;
			case SB_LINEUP:
				delta = -4;
				break;
			case SB_LINEDOWN:
				delta = 4;
				break;
			case SB_PAGEUP:
				delta = -20;
				break;
			case SB_PAGEDOWN:
				delta = 20;
				break;
			}
			if (delta != 0) {
				pos = pScrollBar->GetScrollPos();
				pScrollBar->SetScrollPos(pos + delta);
			}
			Invalidate();
		}
	}
	else { // 윈도우 스크롤바에서 발생한 메시지이면...
		CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CScrollBarView::OnDraw(CDC* pDC)
{
	// 스크롤바의 현재 위치를 얻는다.
	int hpos = m_hsb.GetScrollPos();
	int vpos = m_vsb.GetScrollPos();

	CFont font;
	LOGFONT lf = { 0 }; // 구조체를 0으로 초기화한다.
	lf.lfHeight = vpos / 3 + 1; // 폰트 크기를 설정한다. vpos == 0이면 크기는 1이다.
	lf.lfEscapement = 10 * hpos; // 폰트 회전각을 설정한다.
	font.CreateFontIndirect(&lf); // 폰트를 생성한다.
	pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0, vpos, 0)); // 폰트 색상을 설정한다.
	pDC->TextOutW(50, 100, CString(_T("안녕하세요")));

	CString str;
	str.Format(_T("글자 크기: %d, 회전각: %d, 초록색 농도: %d"), lf.lfHeight, hpos, vpos);
	m_status.SetWindowText(str); // 스태틱 텍스트 컨트롤에 출력한다.
}
