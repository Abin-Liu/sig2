// StatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "StatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char g_szTemp[128] = ""; // use an ugly global buffer for the fastest possible access

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog


CStatsDlg::CStatsDlg(LPCSTR lpszThreadName, LPCSIGSTATS pStats, CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(lpszThreadName);
	ASSERT(pStats);

	m_sName = lpszThreadName;
	m_pStats = pStats;
}


void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg message handlers

BOOL CStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sTitle;

	if (g_bLanguage)
	{
		sTitle.Format("线程运行统计 - %s", m_sName);
		SetDlgItemText(IDC_FRA_APPLICATION, "应用程序");
		SetDlgItemText(IDC_LBL_SINCE, "开始时间:");
		SetDlgItemText(IDC_LBL_ASOF, "截止至:");
		SetDlgItemText(IDC_LBL_TOTALRUN, "总计运行:");
		SetDlgItemText(IDC_LBL_INGAME, "游戏内时间:");
		SetDlgItemText(IDC_LBL_CRASHES, "客户端崩溃:");
		SetDlgItemText(IDC_FRA_GAME, "游戏");
		SetDlgItemText(IDC_LBL_GAMES, "创建:");
		SetDlgItemText(IDC_LBL_GAMEOKS, "创建成功:");
		SetDlgItemText(IDC_LBL_CURGAME, "当前游戏时间:");
		SetDlgItemText(IDC_LBL_MAXGAME, "最长游戏时间:");
		SetDlgItemText(IDC_LBL_MINGAME, "最短游戏时间:");
		SetDlgItemText(IDC_LBL_AVERAGEGAME, "平均游戏时间:");
	}
	else
	{
		sTitle.Format("Thread Statistics - %s", m_sName);
	}

	SetWindowText(sTitle);

	SetTimer(ID_TMR_STATS, 1000, NULL);
	Update();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatsDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_STATS)
		Update();

	CDialog::OnTimer(nIDEvent);
}

void CStatsDlg::Update()
{
	if (m_pStats == NULL)
		return;

	if (m_pStats->lStart == 0)
		SetDlgItemText(IDC_SINCE, g_bLanguage ? "<尚未运行>" : "<Not Started>");
	else
		SetDlgItemText(IDC_SINCE, GetTimeStr(m_pStats->lStart));
	SetDlgItemText(IDC_ASOF, GetTimeStr(m_pStats->lStop));
	SetDlgItemText(IDC_RUNNING, GetLengthStr(m_pStats->nTotalRunning));
	SetDlgItemText(IDC_INGAME, GetLengthStr(m_pStats->nTotalInGame, m_pStats->nTotalRunning));
	SetDlgItemText(IDC_CRASHES, GetNumStr(m_pStats->nCrashes));
	SetDlgItemText(IDC_CREATION, GetNumStr(m_pStats->nGames));
	SetDlgItemText(IDC_SUCCESS, GetNumStr(m_pStats->nGameOKs, m_pStats->nGames));
	SetDlgItemText(IDC_CUR, GetLengthStr(m_pStats->nCurGameLen));
	SetDlgItemText(IDC_MAX, GetLengthStr(m_pStats->nMaxGame));
	SetDlgItemText(IDC_MIN, GetLengthStr(m_pStats->nMinGame));
	SetDlgItemText(IDC_AVG, GetLengthStr(int((double)m_pStats->nTotalInGame / (double)m_pStats->nGameOKs)));
}

LPCSTR CStatsDlg::GetLengthStr(int nLength, int nDivisor)
{
	g_szTemp[0] = 0;
	int nSeconds = nLength;
	int nHour = nSeconds / 3600;
	nSeconds %= 3600;
	int nMinute = nSeconds / 60;
	nSeconds %= 60;

	if (nDivisor)
	{
		double f = (double)nLength / (double)nDivisor * 100.0;
		::sprintf(g_szTemp, "%02d:%02d:%02d (%.2f%%)", nHour, nMinute, nSeconds, f);
	}
	else
	{
		::sprintf(g_szTemp, "%02d:%02d:%02d", nHour, nMinute, nSeconds);
	}
	
	return g_szTemp;
}

LPCSTR CStatsDlg::GetTimeStr(time_t time)
{
	g_szTemp[0] = 0;
	if (time == 0)
		::time(&time);

	tm* p = ::localtime(&time);	

	if (g_bLanguage)
		::strftime(g_szTemp, 63, "%Y-%m-%d | %H:%M:%S", p);
	else
		::strftime(g_szTemp, 63, "%b %d, %Y | %H:%M:%S", p);
	return g_szTemp;
}

LPCSTR CStatsDlg::GetNumStr(int nNum, int nDivisor)
{
	g_szTemp[0] = 0;
	if (nDivisor)
	{
		double f = (double)nNum / (double)nDivisor * 100.0;
		::sprintf(g_szTemp, "%d (%.2f%%)", nNum, f);
	}
	else
	{
		::sprintf(g_szTemp, "%d", nNum);
	}
	
	return g_szTemp;
}

void CStatsDlg::OnDestroy() 
{
	KillTimer(ID_TMR_STATS);
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CStatsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN)
	{
		OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);
}
