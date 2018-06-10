// OptPage6.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageJSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const LPCSTR IGNORE_SCRIPTS[] = 
{
		"default",
		"autostarter",
		"starter",
		NULL
};

/////////////////////////////////////////////////////////////////////////////
// CPageJSP property page

IMPLEMENT_DYNCREATE(CPageJSP, CPropertyPage)

CPageJSP::CPageJSP() : CPropertyPage(CPageJSP::IDD)
{
	//{{AFX_DATA_INIT(CPageJSP)
	m_bAllowModify = FALSE;
	//}}AFX_DATA_INIT
}

CPageJSP::~CPageJSP()
{
}

void CPageJSP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageJSP)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Check(pDX, IDC_CHECK1, m_bAllowModify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageJSP, CPropertyPage)
	//{{AFX_MSG_MAP(CPageJSP)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageJSP message handlers

BOOL CPageJSP::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, (WPARAM)"D2JSP",
		(LPARAM)(g_bLanguage ? "选择要加载的D2Jsp脚本。" : "Specify D2JSP data."));
	return CPropertyPage::OnSetActive();
}

void CPageJSP::SetData(CSIGThreadData* pData)
{
	m_pData = pData;
	ASSERT(m_pData);
	m_bAllowModify = pData->bAllowModifyScript;
	SplitString(pData->sD2JSPScripts, ",", m_aModules);
}

BOOL CPageJSP::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.InsertColumn(0, "", LVCFMT_LEFT, 300);
	m_wndList.SetCheckboxeStyle();
	CString str = ListModules(m_pData->sD2JspDll, "Scripts\\Bots", "d2j", m_aModules, &m_wndList, IGNORE_SCRIPTS);
	SetDlgItemText(IDC_LBL_PATH, str);
	m_wndList.EnableWindow(m_bAllowModify);

	if (g_bLanguage)
	{
		SetDlgItemText(IDC_CHECK1, "允许SIG更改和配置Default.d2j脚本文件。");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageJSP::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_pData->bAllowModifyScript = m_bAllowModify;
	m_aModules.RemoveAll();
	const int COUNT = m_wndList.GetItemCount();
	CString sAll;
	for (int i = 0; i < COUNT; i++)
	{
		if (m_wndList.ExamItemStates(i, RC_ITEM_CHECKED))
		{
			m_aModules.Add(m_wndList.GetItemText(i, 0));
			sAll += m_wndList.GetItemText(i, 0);
			sAll += ',';
		}
	}

	sAll.TrimRight(',');
	m_pData->sD2JSPScripts = sAll;
	return CPropertyPage::OnWizardNext();
}

void CPageJSP::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndList.EnableWindow(m_bAllowModify);
}
