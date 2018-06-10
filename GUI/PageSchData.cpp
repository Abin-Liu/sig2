// PageSchData.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageSchData.h"
#include "SChDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSchData property page

IMPLEMENT_DYNCREATE(CPageSchData, CPropertyPage)

CPageSchData::CPageSchData() : CPropertyPage(CPageSchData::IDD)
{
	//{{AFX_DATA_INIT(CPageSchData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSchedule = NULL;
	m_psd = NULL;
}

CPageSchData::~CPageSchData()
{
}

void CPageSchData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSchData)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSchData, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSchData)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSchData message handlers

BOOL CPageSchData::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, 
		(WPARAM)(g_bLanguage ? "计划数据" : "Schedule Data"),
		(LPARAM)(g_bLanguage ? "为这一计划添加或更改计划数据。" : "Add or change information of this schedule."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageSchData::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_aData.RemoveAll();
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		CScheduleData* p = (CScheduleData*)m_wndList.GetItemData(i);
		if (p)
			m_aData.Add(*p);
	}
	return CPropertyPage::OnWizardNext();
}

BOOL CPageSchData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.SetColumnHeader(g_bLanguage ? "任务列表, 120;启动条件,140;终止条件,140" : "Task List, 120;Start Condition,140;Stop Condition,140");
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_ADD, "添加");
		SetDlgItemText(IDC_EDIT, "编辑");
		SetDlgItemText(IDC_REMOVE, "删除");
		SetDlgItemText(IDC_MOVEUP, "上移");
		SetDlgItemText(IDC_MOVEDOWN, "下移");
	}

	if (m_pSchedule)
		m_pSchedule->EnumTasks(EnumTaskProc, (LPARAM)&m_wndList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSchData::SetData(const CSIGData* psd, const CSIGSchedule* pSchedule)
{
	m_pSchedule = pSchedule;
	m_psd = psd;
}

void CPageSchData::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int COUNT = m_wndList.GetSelectedCount();
	GetDlgItem(IDC_REMOVE)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_EDIT)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_MOVEUP)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_MOVEDOWN)->EnableWindow(COUNT == 1);
	*pResult = 0;
}

void CPageSchData::OnAdd() 
{
	// TODO: Add your control notification handler code here
	SChDataDlg dlg(m_psd, NULL, this);
	if (dlg.DoModal() != IDOK)
		return;

	CScheduleData* p = new CScheduleData;
	ASSERT(p);
	*p = *dlg.GetData();

	const int IDX = m_wndList.InsertItem(INT_MAX, p->sThreadName);
	m_wndList.SetItemData(IDX, (DWORD)p);	
	CString sStart, sEnd;
	MakeConditionStr(p, sStart, sEnd);
	m_wndList.SetItemText(IDX, 1, sStart);
	m_wndList.SetItemText(IDX, 2, sEnd);
}

void CPageSchData::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CScheduleData* p = (CScheduleData*)m_wndList.GetItemData(IDX);
	if (p == NULL)
		return;

	SChDataDlg dlg(m_psd, p, this);
	if (dlg.DoModal() != IDOK)
		return;

	*p = *dlg.GetData();
	m_wndList.SetItemText(IDX, 0, p->sThreadName);
	CString sStart, sEnd;
	MakeConditionStr(p, sStart, sEnd);
	m_wndList.SetItemText(IDX, 1, sStart);
	m_wndList.SetItemText(IDX, 2, sEnd);
}

void CPageSchData::OnRemove() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;
	
	CScheduleData* p = (CScheduleData*)m_wndList.GetItemData(IDX);
	if (p)
		delete p;

	m_wndList.SetItemData(IDX, 0);
	m_wndList.DeleteItem(IDX);
}

void CPageSchData::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_wndList.MoveUp(IDX);
}

void CPageSchData::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_wndList.MoveDown(IDX);
}

void CPageSchData::EnumTaskProc(const CScheduleData* pTask, LPARAM lParam)
{
	CReportCtrl* pList = (CReportCtrl*)lParam;
	if (pTask == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;

	CScheduleData* p = new CScheduleData;
	ASSERT(p);
	*p = *pTask;
	const int IDX = pList->InsertItem(INT_MAX, p->sThreadName);
	pList->SetItemData(IDX, (DWORD)p);
	CString sStart, sEnd;
	MakeConditionStr(pTask, sStart, sEnd);
	pList->SetItemText(IDX, 1, sStart);
	pList->SetItemText(IDX, 2, sEnd);
}

void CPageSchData::OnDestroy() 
{
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		CScheduleData* p = (CScheduleData*)m_wndList.GetItemData(i);
		if (p)
			delete p;
		m_wndList.SetItemData(i, 0);
	}

	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here	
}

const CScheduleData* CPageSchData::GetData(int &rCount) const
{
	rCount = m_aData.GetSize();
	return m_aData.GetData();
}

void CPageSchData::MakeConditionStr(const CScheduleData *p, CString &rStart, CString &rStop)
{
	rStart.Empty();
	rStop.Empty();
	if (p == NULL)
		return;

	switch (p->nStartRule)
	{
	case SCH_START_AFTERPREVSTART:
		rStart.Format(g_bLanguage ? "前一个任务启动后%d秒" : "%d seconds after previous task starts", p->nStartParam);
		break;

	case SCH_START_AFTERPREVSTOP:
		rStart.Format(g_bLanguage ? "前一个任务终止后%d秒" : "%d seconds after previous task stops", p->nStartParam);
		break;

	default:
		rStart = g_bLanguage ? "立即启动" : "Immediately";
		break;
	}

	switch (p->nEndRule)
	{
	case SCH_END_LENGTH:
		rStop.Format(g_bLanguage ? "本任务启动后%d秒" : "%d seconds after this task starts", p->nEndParam);
		break;

	case SCH_END_GAMES:
		rStop.Format(g_bLanguage ? "本任务创建游戏数达到%d后" : "Game counter reaches %d", p->nEndParam);
		break;

	default:
		rStop = g_bLanguage ? "从不" : "Never"; 
		break;
	}
}
