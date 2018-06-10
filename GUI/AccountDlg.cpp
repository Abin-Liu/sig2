// AccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "AccountDlg.h"
#include "RealmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog


CAccountDlg::CAccountDlg(const CMuleData* pData, CRealmInfo* pri, CWnd* pParent /*=NULL*/)
	: CDialog(CAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountDlg)
	m_sAccount = _T("");
	m_sBnetIP = _T("");
	m_nCharCount = 7;
	m_sPwd1 = _T("");
	m_sSubRealm = _T("");
	m_sPwd2 = _T("");
	//}}AFX_DATA_INIT
	
	if (pData)
	{
		m_data = *pData;
		m_nCharCount = m_data.nCount - 1;
		m_sAccount = pData->login.sAccount;
		m_sPwd1 = pData->login.sPassword;
		m_sPwd2 = m_sPwd1;
		m_sSubRealm = pData->login.sSubRealm;
		m_sBnetIP = pData->login.sBnetIP;
	}

	m_pri = pri;
	ASSERT(m_pri);
}


void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountDlg)
	DDX_Control(pDX, IDC_REPEATPASSWORD, m_wndPwd2);
	DDX_Control(pDX, IDC_PREFERRED, m_wndSubRealm);
	DDX_Control(pDX, IDC_PASSWORD, m_wndPwd1);
	DDX_Control(pDX, IDC_BNETIP, m_wndBnetIP);
	DDX_Control(pDX, IDC_ACCOUNT, m_wndAccount);
	DDX_Control(pDX, IDC_REALMS, m_wndRealms);
	DDX_Text(pDX, IDC_ACCOUNT, m_sAccount);
	DDV_MaxChars(pDX, m_sAccount, 31);
	DDX_Text(pDX, IDC_BNETIP, m_sBnetIP);
	DDX_CBIndex(pDX, IDC_CHARCOUNT, m_nCharCount);
	DDX_Text(pDX, IDC_PASSWORD, m_sPwd1);
	DDV_MaxChars(pDX, m_sPwd1, 31);
	DDX_Text(pDX, IDC_PREFERRED, m_sSubRealm);
	DDX_Text(pDX, IDC_REPEATPASSWORD, m_sPwd2);
	DDV_MaxChars(pDX, m_sPwd2, 31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountDlg)
	ON_CBN_SELCHANGE(IDC_REALMS, OnSelchangeRealms)
	ON_BN_CLICKED(IDC_REALMEDIT, OnRealmedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg message handlers

BOOL CAccountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetWindowText("登录信息");
		SetDlgItemText(IDOK, "确定");
		SetDlgItemText(IDCANCEL, "取消");
		SetDlgItemText(IDC_FRA_REALM, "登录国度");
		SetDlgItemText(IDC_REALMEDIT, "编辑");
		SetDlgItemText(IDC_FRA_ACCOUNT, "账号信息");
		SetDlgItemText(IDC_LBL_ACCOUNT, "账号:");
		SetDlgItemText(IDC_LBL_PWD, "密码:");
		SetDlgItemText(IDC_LBL_PWD2, "重复密码:");
		SetDlgItemText(IDC_FRA_CHAR, "角色数量");
		SetDlgItemText(IDC_LBL_PLAYS, "希望登录的角色数量:");
	}
	ListRealms();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccountDlg::ListRealms()
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
	
	if (m_sBnetIP.IsEmpty())
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
	else
	{
		char szName[521] = "";
		if (m_pri->FindRealmName(m_sBnetIP, m_sSubRealm, szName))
		{
			m_wndRealms.SetCurSel(m_wndRealms.FindStringExact(0, szName));
		}
	}
}

void CAccountDlg::OnSelchangeRealms() 
{
	// TODO: Add your control notification handler code here
	CString sName;
	m_wndRealms.GetLBText(m_wndRealms.GetCurSel(), sName);
	TCHAR szBnetIP[512] = _T("");
	TCHAR szPreferred[512] = _T("");

	m_pri->FindRealmDetail(sName, szBnetIP, szPreferred);
	SetDlgItemText(IDC_BNETIP, szBnetIP);
	SetDlgItemText(IDC_PREFERRED, szPreferred);
}

void CAccountDlg::OnRealmedit() 
{
	// TODO: Add your control notification handler code here
	CRealmDlg dlg(m_pri, this);
	if (dlg.DoModal() == IDOK)
		ListRealms();
}

void CAccountDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sAccount.TrimLeft();
	m_sAccount.TrimRight();

	if (m_sAccount.IsEmpty())
	{
		MessageBox("Account name cannot be empty!", "Invalid Account Name", MB_ICONEXCLAMATION);
		m_wndAccount.SetFocus();
		m_wndAccount.Highlight();
		return;
	}

	if (m_sPwd1.IsEmpty())
	{
		MessageBox("Password cannot be empty!", "Invalid Password", MB_ICONEXCLAMATION);
		m_wndPwd1.SetFocus();
		m_wndPwd1.Highlight();
		return;
	}

	if (m_sPwd2 != m_sPwd1)
	{
		MessageBox("Password mismatch!", "Invalid Password", MB_ICONEXCLAMATION);
		m_wndPwd2.SetFocus();
		m_wndPwd2.Highlight();
		return;
	}

	m_data.nCount = m_nCharCount + 1;
	m_data.login.sAccount = m_sAccount;
	m_data.login.sBnetIP = m_sBnetIP;
	m_data.login.sSubRealm = m_sSubRealm;
	m_data.login.sPassword = m_sPwd1;

	EndDialog(IDOK);
}

const CMuleData& CAccountDlg::GetData() const
{
	return m_data;
}
