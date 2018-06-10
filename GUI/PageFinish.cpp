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
		(WPARAM)(g_bLanguage ? "�������" : "Finished"),
		(LPARAM)(g_bLanguage ? "������ϣ����\"Finish\"�Ա����������ݡ�" : "We are done! Click \"Finish\" to save changes."));
	return CPropertyPage::OnSetActive();
}

BOOL CPageFinish::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_TEXT, "ף���㣡\n\n���Ѿ��������Ҫ���õ�SIG���ݡ�����\"Finish\"�����������ĸĶ���");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
