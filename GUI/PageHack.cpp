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

	m_wndHackitPath.SetTooltipText(g_bLanguage ? "���" : "Browse");
	m_wndHackitPath.SetDefExt("dll");
	m_wndHackitPath.SetFileFlags(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
	m_wndHackitPath.SetFilter(g_bLanguage ? "DLL�ļ� (*.dll)|*.dll||" : "DLL Files (*.dll)|*.dll||");
	m_wndHackitPath.SetDialogTitle(g_bLanguage ? "ѡ��D2Hackit.dll�ļ�" : "Select D2Hackit.dll File");

	m_wndJspPath.SetTooltipText(g_bLanguage ? "���" : "Browse");
	m_wndJspPath.SetDefExt("dll");
	m_wndJspPath.SetFileFlags(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
	m_wndJspPath.SetFilter(g_bLanguage ? "DLL�ļ� (*.dll)|*.dll||" : "DLL Files (*.dll)|*.dll||");
	m_wndJspPath.SetDialogTitle(g_bLanguage ? "ѡ��D2Jsp.dll�ļ�" : "Select D2Jsp.dll File");
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
		(LPARAM)(g_bLanguage ? "������Hack����ҡ��Լ�αװ����ص���Ϣ��" : "Specify hacks, plugins, and disguising related information."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageHack::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (m_nHackType == 1 && _access(m_wndHackitPath.GetPathName(), 0))
	{
		MessageBox(g_bLanguage ? "��Ч��D2Hackit·����" : "Invalid D2Hackit file path!", "Invalid File", MB_ICONEXCLAMATION);
		m_wndHackitPath.SetFocus();
		return 1;
	}

	if (m_nHackType == 2 && _access(m_wndJspPath.GetPathName(), 0))
	{
		MessageBox(g_bLanguage ? "��Ч��D2Jsp·����" : "Invalid D2JSP file path!", "Invalid File", MB_ICONEXCLAMATION);
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
		SetDlgItemText(IDC_RADIO1, "�������κ����");
		SetDlgItemText(IDC_RADIO2, "����D2Hackit");
		SetDlgItemText(IDC_RADIO3, "����D2Jsp");
		SetDlgItemText(IDC_HACKITURL, "��ҳ");
		SetDlgItemText(IDC_JSPURL, "��ҳ");
		SetDlgItemText(IDC_FRA_GAMES, "��Ϸ");
		SetDlgItemText(IDC_ANTIIDLE, "����ɫ��ֹʱִ��\"Anti-Idle\"ָ�");
		SetDlgItemText(IDC_MINTIME, "������Ϸ֮�����̼��(��):");
		SetDlgItemText(IDC_FRA_BAN, "һ��CDKey��Ban");
		SetDlgItemText(IDC_STOP, "��ֹ�̵߳����С�");
		SetDlgItemText(IDC_RETRY, "�ȴ�������(��):");
		SetDlgItemText(IDC_SWITCH, "�л����߳�:");
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
