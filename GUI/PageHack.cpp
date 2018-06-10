// OptPage4.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageHack.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageHack property page

IMPLEMENT_DYNCREATE(CPageHack, CPropertyPage)

CPageHack::CPageHack() : CPropertyPage(CPageHack::IDD)
{
	//{{AFX_DATA_INIT(CPageHack)
	m_nHackType = 0;
	m_bAntiIdle = FALSE;
	m_bMin2Games = FALSE;
	m_nBanRule = -1;
	m_nMin2Games = 0;
	m_nBanRetry = 0;
	m_sSwitchThread = _T("");
	//}}AFX_DATA_INIT

	m_psd = NULL;
	m_pData = NULL;

	m_wndHackitPath.SetTooltipText(g_bLanguage ? "浏览" : "Browse");
	m_wndHackitPath.SetDefExt("dll");
	m_wndHackitPath.SetFileFlags(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
	m_wndHackitPath.SetFilter(g_bLanguage ? "DLL文件 (*.dll)|*.dll||" : "DLL Files (*.dll)|*.dll||");
	m_wndHackitPath.SetDialogTitle(g_bLanguage ? "选择D2Hackit.dll文件" : "Select D2Hackit.dll File");

	m_wndJspPath.SetTooltipText(g_bLanguage ? "浏览" : "Browse");
	m_wndJspPath.SetDefExt("dll");
	m_wndJspPath.SetFileFlags(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
	m_wndJspPath.SetFilter(g_bLanguage ? "DLL文件 (*.dll)|*.dll||" : "DLL Files (*.dll)|*.dll||");
	m_wndJspPath.SetDialogTitle(g_bLanguage ? "选择D2Jsp.dll文件" : "Select D2Jsp.dll File");
}

CPageHack::~CPageHack()
{
}

void CPageHack::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageHack)
	DDX_Control(pDX, IDC_COMBO1, m_wndCombo);
	DDX_Control(pDX, IDC_BANRETRY, m_wndBanRetry);
	DDX_Control(pDX, IDC_MIN2GAMES, m_wndMin2Games);
	DDX_Control(pDX, IDC_JSPURL, m_wndJspUrl);
	DDX_Control(pDX, IDC_JSPPATH, m_wndJspPath);
	DDX_Control(pDX, IDC_HACKITURL, m_wndHackitUrl);
	DDX_Control(pDX, IDC_HACKITPATH, m_wndHackitPath);
	DDX_Radio(pDX, IDC_RADIO1, m_nHackType);
	DDX_Check(pDX, IDC_ANTIIDLE, m_bAntiIdle);
	DDX_Check(pDX, IDC_MINTIME, m_bMin2Games);
	DDX_Radio(pDX, IDC_STOP, m_nBanRule);
	DDX_Text(pDX, IDC_MIN2GAMES, m_nMin2Games);
	DDX_Text(pDX, IDC_BANRETRY, m_nBanRetry);
	DDX_CBString(pDX, IDC_COMBO1, m_sSwitchThread);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageHack, CPropertyPage)
	//{{AFX_MSG_MAP(CPageHack)
	ON_BN_CLICKED(IDC_RADIO1, OnStatsChange)
	ON_BN_CLICKED(IDC_RADIO2, OnStatsChange)
	ON_BN_CLICKED(IDC_RADIO3, OnStatsChange)
	ON_BN_CLICKED(IDC_MINTIME, OnStatsChange)
	ON_BN_CLICKED(IDC_STOP, OnStatsChange)
	ON_BN_CLICKED(IDC_RETRY, OnStatsChange)
	ON_BN_CLICKED(IDC_SWITCH, OnStatsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageHack message handlers

BOOL CPageHack::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, (WPARAM)"Hacks",
		(LPARAM)(g_bLanguage ? "设置与Hack、外挂、以及伪装等相关的信息。" : "Specify hacks, plugins, and disguising related information."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageHack::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (m_nHackType == 1 && _access(m_wndHackitPath.GetPathName(), 0))
	{
		MessageBox(g_bLanguage ? "无效的D2Hackit路径！" : "Invalid D2Hackit file path!", "Invalid File", MB_ICONEXCLAMATION);
		m_wndHackitPath.SetFocus();
		return 1;
	}

	if (m_nHackType == 2 && _access(m_wndJspPath.GetPathName(), 0))
	{
		MessageBox(g_bLanguage ? "无效的D2Jsp路径！" : "Invalid D2JSP file path!", "Invalid File", MB_ICONEXCLAMATION);
		m_wndJspPath.SetFocus();
		return 1;
	}

	m_sSwitchThread.TrimLeft();
	m_sSwitchThread.TrimRight();

	m_pData->nHackType = m_nHackType;
	m_pData->sD2HackitDll = m_wndHackitPath.GetPathName();
	m_pData->sD2JspDll = m_wndJspPath.GetPathName();
	m_pData->bAntiIdle = m_bAntiIdle;
	m_pData->nMin2Games = m_bMin2Games ? max(0, m_nMin2Games) : 0;
	m_pData->nBanRule = m_nBanRule;
	m_pData->nBanRetry = m_nBanRetry;
	m_pData->sBanSwitchThread = m_sSwitchThread;
	if (m_nBanRule == BAN_SWITCH && m_sSwitchThread.IsEmpty())
		m_pData->nBanRule = BAN_STOP;


	GetParent()->SendMessage(WM_HACK_TYPE, 0, m_nHackType);
	return CPropertyPage::OnWizardNext();
}

void CPageHack::SetData(const CSIGData* psd, CSIGThreadData* pData)
{
	m_psd = psd;
	m_pData = pData;
	ASSERT(m_pData);
	ASSERT(m_psd);
	m_nHackType = pData->nHackType;
	m_wndHackitPath.SetPathName(pData->sD2HackitDll);
	m_wndJspPath.SetPathName(pData->sD2JspDll);
	m_bAntiIdle = pData->bAntiIdle;
	m_bMin2Games = pData->nMin2Games > 0;
	m_nMin2Games = pData->nMin2Games;
	m_nBanRule = pData->nBanRule;
	m_nBanRetry = pData->nBanRetry;
	m_sSwitchThread = pData->sBanSwitchThread;
	if (m_nBanRule == BAN_SWITCH && m_sSwitchThread.IsEmpty())
		m_nBanRule = BAN_STOP;
}

BOOL CPageHack::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndHackitPath.SetPathName(m_pData->sD2HackitDll);
	m_wndJspPath.SetPathName(m_pData->sD2JspDll);
	m_wndHackitUrl.SetURL("http://www.wxjindu.com/abin/d2hackit");
	m_wndJspUrl.SetURL("http://www.d2jsp.org");
	m_wndHackitPath.EnableWindow(m_nHackType == 1);
	m_wndJspPath.EnableWindow(m_nHackType == 2);

	if (g_bLanguage)
	{
		SetDlgItemText(IDC_RADIO1, "不加载任何外挂");
		SetDlgItemText(IDC_RADIO2, "加载D2Hackit");
		SetDlgItemText(IDC_RADIO3, "加载D2Jsp");
		SetDlgItemText(IDC_HACKITURL, "主页");
		SetDlgItemText(IDC_JSPURL, "主页");
		SetDlgItemText(IDC_FRA_GAMES, "游戏");
		SetDlgItemText(IDC_ANTIIDLE, "当角色静止时执行\"Anti-Idle\"指令。");
		SetDlgItemText(IDC_MINTIME, "两个游戏之间的最短间隔(秒):");
		SetDlgItemText(IDC_FRA_BAN, "一旦CDKey被Ban");
		SetDlgItemText(IDC_STOP, "终止线程的运行。");
		SetDlgItemText(IDC_RETRY, "等待后重试(秒):");
		SetDlgItemText(IDC_SWITCH, "切换到线程:");
	}

	m_psd->EnumInstances(EnumInstProc, (LPARAM)&m_wndCombo);
	CheckStats();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageHack::EnumInstProc(const CSIGInstance *pInstance, LPARAM lParam)
{
	CComboBox* pBox = (CComboBox*)lParam;
	if (pInstance == NULL || pBox == NULL || !::IsWindow(pBox->m_hWnd))
		return;
	
	pBox->AddString(pInstance->GetName());
}

void CPageHack::CheckStats()
{
	m_wndHackitPath.EnableWindow(m_nHackType == 1);
	m_wndJspPath.EnableWindow(m_nHackType == 2);
	m_wndMin2Games.EnableWindow(m_bMin2Games);
	m_wndBanRetry.EnableWindow(m_nBanRule == BAN_RETRY);
	m_wndCombo.EnableWindow(m_nBanRule == BAN_SWITCH);
}

void CPageHack::OnStatsChange() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CheckStats();
}
