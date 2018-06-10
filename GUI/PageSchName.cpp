// PageSchName.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageSchName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSchName property page

IMPLEMENT_DYNCREATE(CPageSchName, CPropertyPage)

CPageSchName::CPageSchName() : CPropertyPage(CPageSchName::IDD)
{
	//{{AFX_DATA_INIT(CPageSchName)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}

CPageSchName::~CPageSchName()
{
}

void CPageSchName::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSchName)
	DDX_Control(pDX, IDC_EDIT1, m_wndName);
	DDX_Text(pDX, IDC_EDIT1, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSchName, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSchName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSchName message handlers

BOOL CPageSchName::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, 
		(WPARAM)(g_bLanguage ? "计划名称" : "Schedule Name"),
		(LPARAM)(g_bLanguage ? "设置计划名称。" : "Specify a name for this schedule."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageSchName::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!UpdateData(TRUE))
		return 1;

	m_sName.TrimLeft();
	m_sName.TrimRight();

	BOOL bOK = TRUE;
	if (m_sName.IsEmpty())
	{
		bOK = FALSE;
		MessageBox(g_bLanguage ? "计划名称不能为空！" : "Schedule name cannot be empty!", "Invalid Schedule Name", MB_ICONEXCLAMATION);
	}
	
	if (!bOK)
	{
		m_wndName.SetFocus();
		m_wndName.Highlight();
		return 1;
	}

	return CPropertyPage::OnWizardNext();
}

BOOL CPageSchName::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_LBL_NAME, "SIG线程名称:");
		SetDlgItemText(IDC_LBL_TEXT, "一个\"SIG计划\"是一个能自动启动、监控和终止一系列线程地计划任务。\n\n你可以使用任何文字来命名你的计划。");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSchName::SetData(LPCSTR lpszName)
{
	m_sName = lpszName ? lpszName : "";
}
