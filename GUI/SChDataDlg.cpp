// SChDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "SChDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SChDataDlg dialog


SChDataDlg::SChDataDlg(const CSIGData* psd, const CScheduleData* pData, CWnd* pParent /*=NULL*/)
	: CDialog(SChDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SChDataDlg)
	m_nStartRule = -1;
	m_nStopRule = -1;
	m_nStartParam1 = 0;
	m_nStartParam2 = 0;
	m_nStopParam1 = 0;
	m_nStopParam2 = 0;
	//}}AFX_DATA_INIT

	m_psd = psd;
	if (pData)
		m_data = *pData;

	m_nStartRule = m_data.nStartRule;
	m_nStopRule = m_data.nEndRule;
	m_nStartParam1 = m_data.nStartParam;
	m_nStartParam2 = m_data.nStartParam;
	m_nStopParam1 = m_data.nEndParam;
	m_nStopParam2 = m_data.nEndParam;
}

void SChDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SChDataDlg)
	DDX_Control(pDX, IDC_STOPPARA2, m_wndStopParam2);
	DDX_Control(pDX, IDC_STOPPARA1, m_wndStopParam1);
	DDX_Control(pDX, IDC_STARTPARA2, m_wndStartParam2);
	DDX_Control(pDX, IDC_STARTPARA1, m_wndStartParam1);
	DDX_Control(pDX, IDC_COMBO1, m_wndCombo);
	DDX_Radio(pDX, IDC_STARTCOND1, m_nStartRule);
	DDX_Radio(pDX, IDC_STOPCOND1, m_nStopRule);
	DDX_Text(pDX, IDC_STARTPARA1, m_nStartParam1);
	DDX_Text(pDX, IDC_STARTPARA2, m_nStartParam2);
	DDX_Text(pDX, IDC_STOPPARA1, m_nStopParam1);
	DDX_Text(pDX, IDC_STOPPARA2, m_nStopParam2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SChDataDlg, CDialog)
	//{{AFX_MSG_MAP(SChDataDlg)
	ON_BN_CLICKED(IDC_STARTCOND1, OnConditions)
	ON_BN_CLICKED(IDC_STARTCOND2, OnConditions)
	ON_BN_CLICKED(IDC_STARTCOND3, OnConditions)
	ON_BN_CLICKED(IDC_STOPCOND1, OnConditions)
	ON_BN_CLICKED(IDC_STOPCOND2, OnConditions)
	ON_BN_CLICKED(IDC_STOPCOND3, OnConditions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SChDataDlg message handlers

BOOL SChDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetWindowText("计划任务设置");
		SetDlgItemText(IDOK, "确定");
		SetDlgItemText(IDCANCEL, "取消");
		SetDlgItemText(IDC_LBL_THREAD, "线程:");
		SetDlgItemText(IDC_LBL_START, "启动条件");
		SetDlgItemText(IDC_LBL_STOP, "终止条件");
		SetDlgItemText(IDC_STARTCOND1, "立即启动");
		SetDlgItemText(IDC_STARTCOND2, "前一个任务启动后(秒):");
		SetDlgItemText(IDC_STARTCOND3, "前一个任务终止后(秒):");
		SetDlgItemText(IDC_STOPCOND1, "从不");
		SetDlgItemText(IDC_STOPCOND2, "本任务启动后(秒):");
		SetDlgItemText(IDC_STOPCOND3, "本任务创建游戏数达到:");
	}

	ASSERT(m_psd);
	m_psd->EnumInstances(EnumInstProc, (LPARAM)&m_wndCombo);
	int nIdx = m_wndCombo.FindStringExact(0, m_data.sThreadName);
	m_wndCombo.SetCurSel(nIdx);
	
	CheckStats();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SChDataDlg::EnumInstProc(const CSIGInstance *pInstance, LPARAM lParam)
{
	CComboBox* pBox = (CComboBox*)lParam;
	if (pInstance == NULL || pBox == NULL || !::IsWindow(pBox->m_hWnd))
		return;

	pBox->AddString(pInstance->GetName());
}

void SChDataDlg::CheckStats()
{
	m_wndStartParam1.EnableWindow(m_nStartRule == 1);
	m_wndStartParam2.EnableWindow(m_nStartRule == 2);
	m_wndStopParam1.EnableWindow(m_nStopRule == 1);
	m_wndStopParam2.EnableWindow(m_nStopRule == 2);
}

void SChDataDlg::OnConditions() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void SChDataDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	const int IDX = m_wndCombo.GetCurSel();
	if (IDX == -1)
	{
		MessageBox(g_bLanguage ? "请选择一个线程！" : "Please select a thread!", "Invalid Thread Name", MB_ICONEXCLAMATION);
		m_wndCombo.SetFocus();
		return;
	}

	m_wndCombo.GetLBText(IDX, m_data.sThreadName);
	m_data.nStartRule = m_nStartRule;
	m_data.nEndRule = m_nStopRule;
	m_data.nStartParam = 0;
	m_data.nEndParam = 0;

	if (m_nStartRule == 1)
		m_data.nStartParam = m_nStartParam1;
	else if (m_nStartRule == 2)
		m_data.nStartParam = m_nStartParam2;

	if (m_nStopRule == 1)
		m_data.nEndParam = m_nStopParam1;
	else if (m_nStopRule == 2)
		m_data.nEndParam = m_nStopParam2;

	EndDialog(IDOK);
}

const CScheduleData* SChDataDlg::GetData() const
{
	return &m_data;
}
