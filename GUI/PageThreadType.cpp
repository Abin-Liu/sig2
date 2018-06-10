// PageThreadType.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageThreadType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageThreadType property page

IMPLEMENT_DYNCREATE(CPageThreadType, CPropertyPage)

CPageThreadType::CPageThreadType() : CPropertyPage(CPageThreadType::IDD)
{
	//{{AFX_DATA_INIT(CPageThreadType)
	m_nType = 0;
	//}}AFX_DATA_INIT
	m_pData = NULL;
}

CPageThreadType::~CPageThreadType()
{
}

void CPageThreadType::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageThreadType)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Radio(pDX, IDC_NORMAL, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageThreadType, CPropertyPage)
	//{{AFX_MSG_MAP(CPageThreadType)
	ON_BN_CLICKED(IDC_NORMAL, OnType)
	ON_BN_CLICKED(IDC_MULE, OnType)
	ON_BN_CLICKED(IDC_COPY, OnType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageThreadType message handlers

BOOL CPageThreadType::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.SetImageList(IDB_ITEM);
	m_wndList.SetGridLines();
	m_wndList.InsertColumn(0, g_bLanguage ? "现有线程" : "Existing Threads", LVCFMT_LEFT, 300);
	ASSERT(m_pData);
	GetDlgItem(IDC_COPY)->EnableWindow(m_pData->GetThreadCount());
	m_pData->EnumInstances(EnumThreadProc, (LPARAM)&m_wndList);
	m_wndList.EnableWindow(FALSE);

	if (g_bLanguage)
	{
		SetDlgItemText(IDC_NORMAL, "创建一个常规线程。");
		SetDlgItemText(IDC_MULE, "创建一个\"Mule Mode\"线程。");
		SetDlgItemText(IDC_COPY, "以某个现有的线程作为模板。");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageThreadType::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT,
		(WPARAM)(g_bLanguage ? "创建线程" : "Create Thread"),
		(LPARAM)(g_bLanguage ? "设置线程的创建方式。" : "Specify how the new thread is being created."));

	return CPropertyPage::OnSetActive();
}

LRESULT CPageThreadType::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class	
	UpdateData(TRUE);
	DWORD dwData = 0;
	if (m_nType == 2)
	{
		int nIdx = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
		if (nIdx == -1)
		{
			MessageBox(g_bLanguage ? "请从列表中选择一个线程作为模板。" : "Please select a thread from the list as template.", "Template Thread", MB_ICONEXCLAMATION);
			return 1;
		}

		dwData = m_wndList.GetItemData(nIdx);
	}
	
	GetParent()->SendMessage(WM_PAGE_THREADTYPE, m_nType, dwData);
	return CPropertyPage::OnWizardNext();
}

void CPageThreadType::SetData(const CSIGData *pData)
{
	m_pData = pData;
	ASSERT(m_pData);
}

void CPageThreadType::EnumThreadProc(const CSIGInstance *pInstance, LPARAM lParam)
{
	CReportCtrl* pList = (CReportCtrl*)lParam;
	if (pInstance == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;

	const int IDX = pList->InsertItem(INT_MAX, pInstance->GetName());
	pList->SetItemData(IDX, (DWORD)pInstance);
	pList->SetItemImage(IDX, 0, pInstance->GetMule() ? 0 : 1);
}

void CPageThreadType::OnType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndList.EnableWindow(m_nType == 2);
}
