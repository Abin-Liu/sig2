// OptPage3.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageGame property page

IMPLEMENT_DYNCREATE(CPageGame, CPropertyPage)

CPageGame::CPageGame() : CPropertyPage(CPageGame::IDD)
{
	//{{AFX_DATA_INIT(CPageGame)
	m_sChannel = _T("");
	m_nMaxInGame = 0;
	m_nDifficulty = 0;
	m_sFixedPwd = _T("");
	m_nNameRule = 0;
	m_sPrefix = _T("");
	m_nFixedPwd = 0;
	//}}AFX_DATA_INIT
}

CPageGame::~CPageGame()
{
}

void CPageGame::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGame)
	DDX_Control(pDX, IDC_PREFIX, m_wndPrefix);
	DDX_Control(pDX, IDC_FIXEDPWD, m_wndFixedPwd);
	DDX_Control(pDX, IDC_MAXINGAME, m_wndMaxInGame);
	DDX_Control(pDX, IDC_CHANNEL, m_wndChannel);
	DDX_Text(pDX, IDC_CHANNEL, m_sChannel);
	DDX_Text(pDX, IDC_MAXINGAME, m_nMaxInGame);
	DDV_MinMaxInt(pDX, m_nMaxInGame, 0, 99999999);
	DDX_Radio(pDX, IDC_RADIO1, m_nDifficulty);
	DDX_Text(pDX, IDC_FIXEDPWD, m_sFixedPwd);
	DDV_MaxChars(pDX, m_sFixedPwd, 15);
	DDX_Radio(pDX, IDC_NAMETYPE, m_nNameRule);
	DDX_Text(pDX, IDC_PREFIX, m_sPrefix);
	DDV_MaxChars(pDX, m_sPrefix, 10);
	DDX_Radio(pDX, IDC_USEFIXPWD, m_nFixedPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGame, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGame)
	ON_BN_CLICKED(IDC_NAMETYPE, OnNametype)
	ON_BN_CLICKED(IDC_USEFIXPWD, OnPwdType)
	ON_BN_CLICKED(IDC_RADIO5, OnNametype)
	ON_BN_CLICKED(IDC_RADIO6, OnNametype)
	ON_BN_CLICKED(IDC_RADIO7, OnPwdType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGame message handlers

BOOL CPageGame::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT,
		(WPARAM)(g_bLanguage ? "��Ϸ����" : "Game Control"),
		(LPARAM)(g_bLanguage ? "������Ϸ������Ϣ��" : "Specify game control information."));
	return CPropertyPage::OnSetActive();
}

void CPageGame::SetData(CSIGThreadData* pData)
{
	m_pData = pData;
	ASSERT(m_pData);

	m_nDifficulty = pData->nDifficulty;
	m_nMaxInGame = pData->nInGameLimit;
	m_sChannel = pData->sChannel;
	m_nNameRule = pData->nameRule.nRule;
	m_nFixedPwd = !!pData->nameRule.bUseFixedPwd;
	m_sFixedPwd = pData->nameRule.szFixedPwd;
	m_sPrefix = pData->nameRule.szNamePrefix;
}

LRESULT CPageGame::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	if (m_nFixedPwd && m_sFixedPwd.IsEmpty())
	{
		CString sMsg;		
		if (g_bLanguage)
			sMsg = "����ѡ��ʹ�ù̶����룬�������ṩ������Ϊ�գ��⽫�����㴴������Ϸ��ʹ���κ����롣\n\nȷ����";
		else
			sMsg = "You've chosen to use fixed password but the password you provided is blank. This will force your games being created with no passwords.\n\nDo you want to proceed?";
		if (MessageBox(sMsg, "Game Passwords", MB_ICONEXCLAMATION | MB_YESNO) != IDYES)
		{
			m_wndFixedPwd.SetFocus();
			m_wndFixedPwd.Highlight();
			return 1;
		}
	}

	m_pData->nDifficulty = m_nDifficulty;
	m_pData->nInGameLimit = m_nMaxInGame;
	m_sChannel.TrimLeft();
	m_sChannel.TrimRight();
	m_pData->sChannel = m_sChannel;

	m_sPrefix.TrimLeft();
	m_sPrefix.TrimRight();
	
	m_pData->nameRule.bUseFixedPwd = m_nFixedPwd;
	m_pData->nameRule.nRule = m_nNameRule;
	::strcpy(m_pData->nameRule.szFixedPwd, m_sFixedPwd);
	::strcpy(m_pData->nameRule.szNamePrefix, m_sPrefix);
	return CPropertyPage::OnWizardNext();
}

BOOL CPageGame::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetDlgItemText(IDC_LBL_CHANNEL, "��¼Ƶ��:");
		SetDlgItemText(IDC_LBL_DIFFICULTY, "��Ϸ�Ѷ�");
		SetDlgItemText(IDC_RADIO1, "����");
		SetDlgItemText(IDC_RADIO2, "��ͨ");
		SetDlgItemText(IDC_RADIO3, "ج��");
		SetDlgItemText(IDC_RADIO4, "����");
		SetDlgItemText(IDC_LBL_MAXSTAY, "���Ϸʱ��(��):");
		SetDlgItemText(IDC_LBL_LIMIT, "(0 = ������)");
		SetDlgItemText(IDC_FRA_NAME, "��Ϸ����");
		SetDlgItemText(IDC_NAMETYPE, "�����");
		SetDlgItemText(IDC_RADIO5, "�̶�ǰ׺:");
		SetDlgItemText(IDC_RADIO6, "ʹ���Զ���ʵ�(�����ȡ)��");
		SetDlgItemText(IDC_RADIO8, "ʹ���Զ���ʵ�(����ѭ��)��");
		SetDlgItemText(IDC_FRA_PWD, "��Ϸ����");
		SetDlgItemText(IDC_USEFIXPWD, "�����");
		SetDlgItemText(IDC_RADIO7, "�̶�:");
		//SetDlgItemText(IDC_LBL_TEXT, "���ý�ɫ��һ����Ϸ�е��ͣ��ʱ��(��)������������ʱ�ޣ�SIG����ǿ�аѽ�ɫ�˳���Ϸ��");
	}
	CheckStats();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageGame::OnNametype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void CPageGame::OnPwdType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void CPageGame::CheckStats()
{
	m_wndPrefix.EnableWindow(m_nNameRule == 1);
	m_wndFixedPwd.EnableWindow(m_nFixedPwd);
}
