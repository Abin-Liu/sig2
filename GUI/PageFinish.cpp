// OptPage7.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageFinish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageFinish property page

IMPLEMENT_DYNCREATE(CPageFinish, CPropertyPage)

CPageFinish::CPageFinish() : CPropertyPage(CPageFinish::IDD)
{
	//{{AFX_DATA_INIT(CPageFinish)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageFinish::~CPageFinish()
{
}

void CPageFinish::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageFinish)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageFinish, CPropertyPage)
	//{{AFX_MSG_MAP(CPageFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFinish message handlers

BOOL CPageFinish::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_FINISH); 
	GetParent()->SendMessage(WM_BANNER_TEXT,
		(WPARAM)(g_bLanguage ? "完成设置" : "Finished"),
		(LPARAM)(g_bLanguage ? "设置完毕！点击\"Finish\"以保存设置内容。" : "We are done! Click \"Finish\" to save changes."));
	return CPropertyPage::OnSetActive();
}

BOOL CPageFinish::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_TEXT, "祝贺你！\n\n你已经完成了所要设置的SIG数据。请点击\"Finish\"来保存所作的改动。");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
