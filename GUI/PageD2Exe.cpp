// OptPage2.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageD2Exe.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageD2Exe property page

IMPLEMENT_DYNCREATE(CPageD2Exe, CPropertyPage)

CPageD2Exe::CPageD2Exe() : CPropertyPage(CPageD2Exe::IDD)
{
	//{{AFX_DATA_INIT(CPageD2Exe)
	m_sParams = _T("");
	//}}AFX_DATA_INIT

	m_wndBrowse.SetTooltipText(g_bLanguage ? "浏览" : "Browse");
	m_wndBrowse.SetDefExt("exe");
	m_wndBrowse.SetFileFlags(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
	m_wndBrowse.SetFilter(g_bLanguage ? "程序文件 (*.exe)|*.exe||" : "Program Files (*.exe)|*.exe||");
	m_wndBrowse.SetDialogTitle(g_bLanguage ? "选择Diablo II程序文件" : "Select Diablo II Program File");
}

CPageD2Exe::~CPageD2Exe()
{
}

void CPageD2Exe::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageD2Exe)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_wndBrowse);
	DDX_Text(pDX, IDC_EDIT1, m_sParams);
	DDV_MaxChars(pDX, m_sParams, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageD2Exe, CPropertyPage)
	//{{AFX_MSG_MAP(CPageD2Exe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageD2Exe message handlers

BOOL CPageD2Exe::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	CString s1, s2;
	s1 = g_bLanguage ? "Diablo II游戏程序" : "Diablo II Program";
	s2 = g_bLanguage ? "选择Diablo II游戏程序路径和命令行参数。" : "Specify Diablo II game program and parameters.";
	GetParent()->SendMessage(WM_BANNER_TEXT, (WPARAM)(LPCSTR)s1, (LPARAM)(LPCSTR)s2);
	return CPropertyPage::OnSetActive();
}

LRESULT CPageD2Exe::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CString str = m_wndBrowse.GetPathName();
	str.TrimLeft();
	str.TrimRight();

	if (str.IsEmpty() || _access(str, 0))
	{
		MessageBox(g_bLanguage ? "请选择一个合法的可执行文件！" : "Please select a valid file!", "Invalid File Name", MB_ICONEXCLAMATION);
		m_wndBrowse.SetFocus();
		return 1;
	}

	m_sParams.TrimLeft();
	m_sParams.TrimRight();

	m_pData->sD2Executable = str;
	m_pData->sParamLine = m_sParams;

	return CPropertyPage::OnWizardNext();
}

void CPageD2Exe::SetData(CSIGThreadData* pData)
{
	m_pData = pData;
	ASSERT(m_pData);
	m_wndBrowse.SetPathName(pData->sD2Executable);
	m_sParams = pData->sParamLine;
}

BOOL CPageD2Exe::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndBrowse.SetPathName(m_pData->sD2Executable);
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_LBL_D2EXE, "Diablo II游戏可执行文件路径:");
		SetDlgItemText(IDC_LBL_PARAM, "命令行参数:");
		SetDlgItemText(IDC_LBL_TEXT, "请勿使用\"-w\"、\"-skiptobnet\"和\"-nonotify\"参数，因为这些参数会被自动强制加入。");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
