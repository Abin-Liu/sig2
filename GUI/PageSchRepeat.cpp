// PageSchRepeat.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageSchRepeat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSchRepeat property page

IMPLEMENT_DYNCREATE(CPageSchRepeat, CPropertyPage)

CPageSchRepeat::CPageSchRepeat() : CPropertyPage(CPageSchRepeat::IDD)
{
	//{{AFX_DATA_INIT(CPageSchRepeat)
	m_nRepeat = 2;
	m_nType = 0;
	//}}AFX_DATA_INIT
}

CPageSchRepeat::~CPageSchRepeat()
{
}

void CPageSchRepeat::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSchRepeat)
	DDX_Control(pDX, IDC_RPT, m_wndRpt);
	DDX_Text(pDX, IDC_RPT, m_nRepeat);
	DDV_MinMaxInt(pDX, m_nRepeat, 2, 65535);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSchRepeat, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSchRepeat)
	ON_BN_CLICKED(IDC_RADIO1, OnType)
	ON_BN_CLICKED(IDC_RADIO2, OnType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSchRepeat message handlers

void CPageSchRepeat::SetData(int nRepeat)
{
	m_nRepeat = max(2, nRepeat);
	if (nRepeat < 2)
		m_nType = 0;
	else
		m_nType = 1;
}

int CPageSchRepeat::GetRepeat() const
{
	return m_nType == 1 ? m_nRepeat : 1;
}

BOOL CPageSchRepeat::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, 
		(WPARAM)(g_bLanguage ? "计划运行" : "Schedule Running"),
		(LPARAM)(g_bLanguage ? "选择计划运行方式。" : "Specify schedule running method."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageSchRepeat::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!UpdateData(TRUE))
		return 1;
	return CPropertyPage::OnWizardNext();
}

void CPageSchRepeat::OnType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndRpt.EnableWindow(m_nType);
}

BOOL CPageSchRepeat::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_FRA_REPEAT, "计划重复");
		SetDlgItemText(IDC_RADIO1, "仅运行一次。");
		SetDlgItemText(IDC_RADIO2, "重复(次):");
	}
	m_wndRpt.EnableWindow(m_nType);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
