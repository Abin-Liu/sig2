// OptPageLogin.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageLogin.h"
#include "RealmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageLogin property page

IMPLEMENT_DYNCREATE(CPageLogin, CPropertyPage)

CPageLogin::CPageLogin() : CPropertyPage(CPageLogin::IDD)
{
	//{{AFX_DATA_INIT(CPageLogin)
	m_sAccount = _T("");
	m_nCharPos = 0;
	m_sPwd1 = _T("");
	m_sPwd2 = _T("");
	m_sSubRealm = _T("");
	m_sBnetIP = _T("");
	//}}AFX_DATA_INIT
}

CPageLogin::~CPageLogin()
{
}

void CPageLogin::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageLogin)
	DDX_Control(pDX, IDC_PREFERRED, m_wndSubRealm);
	DDX_Control(pDX, IDC_BNETIP, m_wndBnetIP);
	DDX_Control(pDX, IDC_REALMS, m_wndRealms);
	DDX_Control(pDX, IDC_REPEATPASSWORD, m_wndPwd2);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPwd1);
	DDX_Control(pDX, IDC_ACCOUNT, m_wndAccount);
	DDX_Text(pDX, IDC_ACCOUNT, m_sAccount);
	DDV_MaxChars(pDX, m_sAccount, 31);
	DDX_Radio(pDX, IDC_P1, m_nCharPos);
	DDX_Text(pDX, IDC_PASSWORD, m_sPwd1);
	DDV_MaxChars(pDX, m_sPwd1, 31);
	DDX_Text(pDX, IDC_REPEATPASSWORD, m_sPwd2);
	DDV_MaxChars(pDX, m_sPwd2, 31);
	DDX_Text(pDX, IDC_PREFERRED, m_sSubRealm);
	DDX_Text(pDX, IDC_BNETIP, m_sBnetIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageLogin, CPropertyPage)
	//{{AFX_MSG_MAP(CPageLogin)
	ON_BN_CLICKED(IDC_REALMEDIT, OnRealmedit)
	ON_CBN_SELCHANGE(IDC_REALMS, OnSelchangeRealms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageLogin message handlers

BOOL CPageLogin::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, 
		(WPARAM)(g_bLanguage ? "登录" : "Login"),
		(LPARAM)(g_bLanguage ? "设置Battle.net国度和登录信息。" : "Specify Battle.net realm and login information."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageLogin::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_sAccount.TrimLeft();
	m_sAccount.TrimRight();

	if (m_sAccount.IsEmpty())
	{
		MessageBox(g_bLanguage ? "账号名不能为空！" : "Account name cannot be empty!", "Invalid Account Name", MB_ICONEXCLAMATION);
		m_wndAccount.SetFocus();
		m_wndAccount.Highlight();
		return 1;
	}

	if (m_sPwd1.IsEmpty())
	{
		MessageBox(g_bLanguage ? "密码不能为空！" : "Password cannot be empty!", "Invalid Password", MB_ICONEXCLAMATION);
		m_wndPwd1.SetFocus();
		m_wndPwd1.Highlight();
		return 1;
	}

	if (m_sPwd2 != m_sPwd1)
	{
		MessageBox(g_bLanguage ? "密码不相符！" : "Password mismatch!", "Invalid Password", MB_ICONEXCLAMATION);
		m_wndPwd2.SetFocus();
		m_wndPwd2.Highlight();
		return 1;
	}

	m_pData->sAccount = m_sAccount;
	m_pData->sBnetIP = m_sBnetIP;
	m_pData->sSubRealm = m_sSubRealm;
	m_pData->sPassword = m_sPwd1;
	m_pData->nCharIdx = m_nCharPos;

	return CPropertyPage::OnWizardNext();
}

void CPageLogin::SetData(CLoginData* pData, CRealmInfo* pri)
{
	m_pData = pData;
	m_pri = pri;
	ASSERT(m_pData && m_pri);

	m_nCharPos = pData->nCharIdx;
	m_sAccount = pData->sAccount;
	m_sPwd1 = pData->sPassword;
	m_sPwd2 = m_sPwd1;
	m_sSubRealm = pData->sSubRealm;
	m_sBnetIP = pData->sBnetIP;
}

void CPageLogin::ListRealms()
{
	int nIndex = 0;
	m_wndRealms.ResetContent();
	m_pri->InitPrivateRealms(AfxGetAppEx()->GetAppDir() + REALM_FILE);
	while (TRUE)
	{
		char szName[521] = "";
		char szPreferred[521] = "";
		char szBnetIP[521] = "";
		if (m_pri->GetRealmByIndex(nIndex++, szName, szBnetIP, szPreferred))
		{
			m_wndRealms.AddString(szName);
		}
		else
		{
			break;
		}
	}

	TCHAR szRealmName[521] = _T("");
	int nItem = -1;
	if (m_pri->FindRealmName(m_sBnetIP, m_sSubRealm, szRealmName))
	{
		nItem = m_wndRealms.FindStringExact(0, szRealmName);
	}

	if (nItem != -1)
	{
		// highlight pre set realm
		m_wndRealms.SetCurSel(nItem);
		SetDlgItemText(IDC_BNETIP, m_sBnetIP);
		SetDlgItemText(IDC_PREFERRED, m_sSubRealm);
	}
	else
	{
		// highlight default realm
		char szPreferred[521] = "";
		char szBnetIP[521] = "";
		if (m_pri->GetDefaultRealm(szBnetIP, szPreferred))
		{
			char szName[512] = "";
			if (m_pri->FindRealmName(szBnetIP, szPreferred, szName))
			{
				m_wndRealms.SetCurSel(m_wndRealms.FindStringExact(0, szName));
			}

			SetDlgItemText(IDC_BNETIP, szBnetIP);
			SetDlgItemText(IDC_PREFERRED, szPreferred);
		}
	}	
}

BOOL CPageLogin::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ListRealms();

	if (g_bLanguage)
	{
		SetDlgItemText(IDC_REALMEDIT, "编辑");
		SetDlgItemText(IDC_LBL_ACCOUNT, "账号:");
		SetDlgItemText(IDC_LBL_PWD, "密码:");
		SetDlgItemText(IDC_LBL_PWD2, "重复密码:");
		SetDlgItemText(IDC_LBL_CHARPOS, "角色位置");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageLogin::OnRealmedit() 
{
	// TODO: Add your control notification handler code here
	CRealmDlg dlg(m_pri, this);
	if (dlg.DoModal() == IDOK)
		ListRealms();
}

void CPageLogin::OnSelchangeRealms() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	m_wndRealms.GetLBText(m_wndRealms.GetCurSel(), sName);
	char szBnetIP[512] = "";
	char szPreferred[512] = "";

	m_pri->FindRealmDetail(sName, szBnetIP, szPreferred);
	SetDlgItemText(IDC_BNETIP, szBnetIP);
	SetDlgItemText(IDC_PREFERRED, szPreferred);
}
