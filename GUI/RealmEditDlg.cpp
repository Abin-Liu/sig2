// RealmEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "RealmEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealmEditDlg dialog


CRealmEditDlg::CRealmEditDlg(LPCSTR lpszName, LPCSTR lpszServer, LPCSTR lpszSub, CWnd* pParent /*=NULL*/)
	: CDialog(CRealmEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealmEditDlg)
	m_sName = lpszName;
	m_sServer = lpszServer;
	m_sSub = lpszSub;
	//}}AFX_DATA_INIT
}


void CRealmEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealmEditDlg)
	DDX_Control(pDX, IDC_SUB, m_wndSub);
	DDX_Control(pDX, IDC_SERVER, m_wndServer);
	DDX_Control(pDX, IDC_NAME, m_wndName);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDX_Text(pDX, IDC_SERVER, m_sServer);
	DDX_Text(pDX, IDC_SUB, m_sSub);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealmEditDlg, CDialog)
	//{{AFX_MSG_MAP(CRealmEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealmEditDlg message handlers

void CRealmEditDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sName.TrimLeft();
	m_sName.TrimRight();
	m_sServer.TrimLeft();
	m_sServer.TrimRight();
	m_sSub.TrimLeft();
	m_sSub.TrimRight();

	if (m_sServer.IsEmpty() || (m_sName.IsEmpty() && m_sSub.IsEmpty()))
		return;

	if (m_sName.IsEmpty())
		m_sName = m_sSub;

	if (m_sSub.IsEmpty())
		m_sSub = m_sName;

	EndDialog(IDOK);
}

BOOL CRealmEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetWindowText("国度信息");
		SetDlgItemText(IDOK, "确定");
		SetDlgItemText(IDCANCEL, "取消");
		SetDlgItemText(IDC_LBL_NAME, "显示名称:");
		SetDlgItemText(IDC_LBL_SERVER, "服务器:");
		SetDlgItemText(IDC_LBL_SUB, "子国度:");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
