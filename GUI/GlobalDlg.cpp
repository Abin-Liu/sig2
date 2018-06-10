// GlobalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "GlobalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalDlg dialog


CGlobalDlg::CGlobalDlg(LPCGLOBALDATA pData, CWnd* pParent /*=NULL*/)
	: CDialog(CGlobalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGlobalDlg)
	//}}AFX_DATA_INIT

	if (pData)
		::memcpy(&m_data, pData, sizeof(GLOBALDATA));
	else
		SetDefaultGlobalData(&m_data);
	Data2Var();
}


void CGlobalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalDlg)
	DDX_Control(pDX, IDC_SPIN1, m_wndSpin);
	DDX_Text(pDX, IDC_MAXD2PROC, m_nMaxD2Process);
	DDV_MinMaxInt(pDX, m_nMaxD2Process, 1, 16);
	DDX_Text(pDX, IDC_LAUNCHDELAY, m_nLaunchDelay);
	DDV_MinMaxInt(pDX, m_nLaunchDelay, 5, 9999);
	DDX_Text(pDX, IDC_MINCONN, m_nConnectDelay);
	DDV_MinMaxInt(pDX, m_nConnectDelay, 5, 9999);
	DDX_Text(pDX, IDC_LOGINDELAY, m_nLoginDelay);
	DDV_MinMaxInt(pDX, m_nLoginDelay, 5, 9999);
	DDX_Text(pDX, IDC_CHARSELDELAY, m_nCharSelDelay);
	DDV_MinMaxInt(pDX, m_nCharSelDelay, 3, 9999);
	DDX_Text(pDX, IDC_JOINDELAY, m_nCreateDelay);
	DDV_MinMaxInt(pDX, m_nCreateDelay, 5, 9999);
	DDX_Text(pDX, IDC_ERRORDELAY, m_nRestartDelay);
	DDV_MinMaxInt(pDX, m_nRestartDelay, 10, 9999);
	DDX_Text(pDX, IDC_RND1, m_nRnd1);
	DDV_MinMaxInt(pDX, m_nRnd1, 0, 9999);
	DDX_Text(pDX, IDC_RND2, m_nRnd2);
	DDV_MinMaxInt(pDX, m_nRnd2, 0, 9999);
	DDX_Text(pDX, IDC_BTNDELAY, m_nButtonDelay);
	DDV_MinMaxInt(pDX, m_nButtonDelay, 2, 9999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGlobalDlg, CDialog)
	//{{AFX_MSG_MAP(CGlobalDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalDlg message handlers

BOOL CGlobalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndSpin.SetRange(1, 16);
	
	if (g_bLanguage)
	{
		SetWindowText("全局设置");
		SetDlgItemText(IDC_LBL_GAMES, "系统");
		SetDlgItemText(IDC_LBL_MAXD2, "系统中最多Diablo II进程数:");
		SetDlgItemText(IDC_LBL_DELAYS, "操作延时(单位: 秒)");
		SetDlgItemText(IDC_LBL_LAUNCH, "从启动游戏到出现连接界面:");
		SetDlgItemText(IDC_LBL_CONNECT, "连接到Battle.net最低耗时:");
		SetDlgItemText(IDC_LBL_LOGIN, "从登录账号到出现角色选择界面:");
		SetDlgItemText(IDC_LBL_CHARSEL, "从选定角色到进入聊天室:");
		SetDlgItemText(IDC_LBL_CREATE, "创建游戏最大等待时限:");
		SetDlgItemText(IDC_LBL_CLOSE, "游戏出错关闭后的等待时间:");
		SetDlgItemText(IDC_DEFAULT, "恢复默认值(&D)");
		SetDlgItemText(IDOK, "确定(&O)");
		SetDlgItemText(IDCANCEL, "取消(&C)");
		SetDlgItemText(IDC_LBL_RND, "游戏创建随机延迟:");
		SetDlgItemText(IDC_LBL_BTN, "[创建]按钮延迟:");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGlobalDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
		return;

	Var2Data();
	EndDialog(IDOK);
}

void CGlobalDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	SetDefaultGlobalData(&m_data);
	Data2Var();
	UpdateData(FALSE);
}

void CGlobalDlg::Data2Var()
{
	m_nMaxD2Process = m_data.nMaxD2Process;
	m_nLaunchDelay = m_data.nLaunchDelay;
	m_nConnectDelay = m_data.nConnectDelay;
	m_nLoginDelay = m_data.nLoginDelay;
	m_nCharSelDelay = m_data.nCharSelDelay;
	m_nCreateDelay = m_data.nCreateDelay;
	m_nRestartDelay = m_data.nRestartDelay;
	m_nRnd1 = m_data.nRand1;
	m_nRnd2 = m_data.nRand2;
	m_nButtonDelay = m_data.nButtonDelay;
}

void CGlobalDlg::Var2Data()
{
	m_data.nMaxD2Process = m_nMaxD2Process;
	m_data.nLaunchDelay = m_nLaunchDelay;
	m_data.nConnectDelay = m_nConnectDelay;
	m_data.nLoginDelay = m_nLoginDelay;
	m_data.nCharSelDelay = m_nCharSelDelay;
	m_data.nCreateDelay = m_nCreateDelay;
	m_data.nRestartDelay = m_nRestartDelay;
	m_data.nRand1 = m_nRnd1;
	m_data.nRand2 = m_nRnd2;
	m_data.nButtonDelay = m_nButtonDelay;
}

LPCGLOBALDATA CGlobalDlg::GetGlobal() const
{
	return &m_data;
}
