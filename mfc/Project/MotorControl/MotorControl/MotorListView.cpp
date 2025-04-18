#include "pch.h"
#include "MotorControl.h"
#include "MotorListView.h"

IMPLEMENT_DYNCREATE(CMotorListView, CFormView)

CMotorListView::CMotorListView()
	: CFormView(IDD_MOTOR_LIST_FORM)
{
	// 뷰가 자식 스타일을 갖도록 설정
	ModifyStyle(0, WS_CHILD | WS_VISIBLE);
}

CMotorListView::~CMotorListView()
{
}

void CMotorListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOTOR_LIST, m_motorList); // 리스트 컨트롤 ID
}

BEGIN_MESSAGE_MAP(CMotorListView, CFormView)
END_MESSAGE_MAP()

void CMotorListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 리스트를 표 스타일로 설정
	m_motorList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 열(column) 추가 (표의 헤더)
	m_motorList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_motorList.InsertColumn(1, _T("모터 이름"), LVCFMT_LEFT, 150);
	m_motorList.InsertColumn(2, _T("상태"), LVCFMT_LEFT, 80);

	// 예시 데이터 삽입
	m_motorList.InsertItem(0, _T("1"));
	m_motorList.SetItemText(0, 1, _T("Main Motor"));
	m_motorList.SetItemText(0, 2, _T("정상"));

	m_motorList.InsertItem(1, _T("2"));
	m_motorList.SetItemText(1, 1, _T("Sub Motor"));
	m_motorList.SetItemText(1, 2, _T("오류"));
}
