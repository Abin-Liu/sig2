// OptPage1.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageName property page

IMPLEMENT_DYNCREATE(CPageName, CPropertyPage)

CPageName::CPageName() : CPropertyPage(CPageName::IDD)
{
	//{{AFX_DATA_INIT(CPageName)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}

CPageName::~CPageName()
{
}

void CPageName::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageName)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT1, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageName, CPropertyPage)
	//{{AFX_MSG_MAP(CPageName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageName message handlers

BOOL CPageName::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class	
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT,
		(WPARAM)(g_bLanguage ? "�߳�����" : "Thread Name"),
		(LPARAM)(g_bLanguage ? "Ϊ���߳�����һ�����ơ�" : "Specify a name for this thread."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageName::OnWizardNext()
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
		MessageBox(g_bLanguage ? "�߳�������Ϊ�գ�" : "Thread name cannot be empty!", "Invalid Thread Name", MB_ICONEXCLAMATION);
	}
	
	if (!bOK)
	{
		m_wndEdit.SetFocus();
		m_wndEdit.Highlight();
		return 1;
	}

	m_pData->sName = m_sName;
	return CPropertyPage::OnWizardNext();
}

void CPageName::SetData(CSIGThreadData* pData)
{
	m_pData = pData;
	ASSERT(m_pData);
	m_sName = pData->sName;
}

BOOL CPageName::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_LBL_NAME, "SIG�߳�����:");
		SetDlgItemText(IDC_LBL_TEXT1, "һ��\"SIG�߳�\"��һ��������SIGӦ�ó���ռ��е���Ϸ����ʵ�塣�����ʹ���κ���������������̡߳�");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
