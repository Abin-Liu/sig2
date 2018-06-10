// OptPage5.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageD2H.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const LPCSTR IGNORE_MODULES[] = 
{
		"sig",
		"d2hackit",
		NULL
};

/////////////////////////////////////////////////////////////////////////////
// CPageD2H property page

IMPLEMENT_DYNCREATE(CPageD2H, CPropertyPage)

CPageD2H::CPageD2H() : CPropertyPage(CPageD2H::IDD)
{
	//{{AFX_DATA_INIT(CPageD2H)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageD2H::~CPageD2H()
{
}

void CPageD2H::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageD2H)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageD2H, CPropertyPage)
	//{{AFX_MSG_MAP(CPageD2H)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageD2H message handlers

BOOL CPageD2H::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, (WPARAM)"D2Hackit", (LPARAM)(g_bLanguage ? "选择要加载的D2Hackit外挂模块。" : "Specify D2Hackit plugin modules."));
	return CPropertyPage::OnSetActive();
}

void CPageD2H::SetData(CSIGThreadData* pData)
{
	m_pData = pData;
	ASSERT(m_pData);
	SplitString(pData->sModules, ",", m_aModules);
}

BOOL CPageD2H::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.InsertColumn(0, "", LVCFMT_LEFT, 300);
	m_wndList.SetCheckboxeStyle();
	CString str = ListModules(m_pData->sD2HackitDll, "", "d2h", m_aModules, &m_wndList, IGNORE_MODULES);
	SetDlgItemText(IDC_LBL_PATH, str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageD2H::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
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
	m_pData->sModules = sAll;
	return CPropertyPage::OnWizardNext();
}
