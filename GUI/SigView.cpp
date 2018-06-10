// SigView.cpp : implementation of the CSigView class
//

#include "stdafx.h"
#include "Sig2.h"
#include "SigView.h"
#include "External\\CmdLine.h"
#include <io.h>
#include "StatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMAGE_NONE		-1
#define IMAGE_MULEMODE	0
#define IMAGE_NORMAL	1
#define IMAGE_STOP		2
#define IMAGE_PAUSE		3
#define IMAGE_START		4
#define IMAGE_ERROR		5
#define IMAGE_CLASS		6
#define IMAGE_SCHEDULE	13

HWND g_hViewWnd = NULL;

/////////////////////////////////////////////////////////////////////////////
// CSigView

CSigView::CSigView()
{
	m_nViewType = 0;	
}

CSigView::~CSigView()
{
	m_data.SaveData(AfxGetAppEx()->GetAppDir() + SIG_FILE, AfxGetAppEx()->GetAppDir() + DICTIONARY_FILE, AfxGetAppEx()->GetAppDir() + REALM_FILE);
}

BEGIN_MESSAGE_MAP(CSigView,CWnd )
	//{{AFX_MSG_MAP(CSigView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_KILL, OnKill)
	ON_UPDATE_COMMAND_UI(ID_KILL, OnUpdateKill)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePause)
	ON_COMMAND(ID_SHOWD2WND, OnShowd2wnd)
	ON_COMMAND(ID_START, OnStart)
	ON_UPDATE_COMMAND_UI(ID_START, OnUpdateStart)
	ON_COMMAND(ID_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_VIEWLOG, OnUpdateViewlog)
	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG, OnUpdateConfig)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_GLOBAL, OnGlobal)	
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_NEW, OnUpdateNew)
	ON_COMMAND(ID_SCH_NEW, OnSchNew)
	ON_UPDATE_COMMAND_UI(ID_SCH_NEW, OnUpdateSchNew)
	ON_NOTIFY(NM_DBLCLK, IDC_THREADLIST, OnDblclkThreadList)
	ON_NOTIFY(NM_DBLCLK, IDC_SCHEDULELIST, OnDblclkScheduleList)
	ON_NOTIFY(NM_RCLICK, IDC_THREADLIST, OnRclickThreadList)
	ON_NOTIFY(NM_RCLICK, IDC_SCHEDULELIST, OnRclickScheduleList)
	ON_COMMAND(ID_SCH_CONFIG, OnSchConfig)
	ON_UPDATE_COMMAND_UI(ID_SCH_CONFIG, OnUpdateSchConfig)
	ON_COMMAND(ID_SCH_START, OnSchStart)
	ON_UPDATE_COMMAND_UI(ID_SCH_START, OnUpdateSchStart)
	ON_COMMAND(ID_SCH_STOP, OnSchStop)
	ON_UPDATE_COMMAND_UI(ID_SCH_STOP, OnUpdateSchStop)
	ON_COMMAND(ID_SCH_DELETE, OnSchDelete)
	ON_UPDATE_COMMAND_UI(ID_SCH_DELETE, OnUpdateSchDelete)
	ON_COMMAND(ID_HIDEWND, OnHidewnd)
	ON_COMMAND(ID_OPT_DICTIONARY, OnOptDictionary)
	ON_COMMAND(ID_THREAD_LOG, OnThreadLog)
	ON_COMMAND(ID_THREAD_STATISTICS, OnThreadStatistics)
	ON_UPDATE_COMMAND_UI(ID_THREAD_STATISTICS, OnUpdateThreadStatistics)
	ON_UPDATE_COMMAND_UI(ID_THREAD_LOG, OnUpdateThreadLog)
	ON_COMMAND(ID_CUS_LOG1, OnCusLog1)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG1, OnUpdateCusLog1)
	ON_COMMAND(ID_CUS_LOG2, OnCusLog2)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG2, OnUpdateCusLog2)
	ON_COMMAND(ID_CUS_LOG3, OnCusLog3)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG3, OnUpdateCusLog3)
	ON_COMMAND(ID_CUS_LOG4, OnCusLog4)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG4, OnUpdateCusLog4)
	ON_COMMAND(ID_SEL_CUSTLOG, OnSelCustlog)
	ON_COMMAND(ID_VIEWLOG, OnViewlog)
	ON_COMMAND(ID_THREAD_LOG1, OnThreadLog)
	ON_COMMAND(ID_KILL1, OnKill)
	ON_UPDATE_COMMAND_UI(ID_KILL1, OnUpdateKill)
	ON_COMMAND(ID_PAUSE1, OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE1, OnUpdatePause)
	ON_COMMAND(ID_SHOWD2WND1, OnShowd2wnd)
	ON_COMMAND(ID_START1, OnStart)
	ON_UPDATE_COMMAND_UI(ID_START1, OnUpdateStart)
	ON_COMMAND(ID_STOP1, OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP1, OnUpdateStop)
	ON_COMMAND(ID_NEW1, OnNew)
	ON_COMMAND(ID_CONFIG1, OnConfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG1, OnUpdateConfig)
	ON_COMMAND(ID_DELETE1, OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE1, OnUpdateDelete)
	ON_COMMAND(ID_GLOBAL1, OnGlobal)	
	ON_UPDATE_COMMAND_UI(ID_NEW1, OnUpdateNew)
	ON_COMMAND(ID_SCH_NEW1, OnSchNew)
	ON_UPDATE_COMMAND_UI(ID_SCH_NEW1, OnUpdateSchNew)
	ON_COMMAND(ID_SCH_CONFIG1, OnSchConfig)
	ON_UPDATE_COMMAND_UI(ID_SCH_CONFIG1, OnUpdateSchConfig)
	ON_COMMAND(ID_SCH_START1, OnSchStart)
	ON_UPDATE_COMMAND_UI(ID_SCH_START1, OnUpdateSchStart)
	ON_COMMAND(ID_SCH_STOP1, OnSchStop)
	ON_UPDATE_COMMAND_UI(ID_SCH_STOP1, OnUpdateSchStop)	
	ON_COMMAND(ID_SCH_DELETE1, OnSchDelete)
	ON_UPDATE_COMMAND_UI(ID_SCH_DELETE1, OnUpdateSchDelete)
	ON_UPDATE_COMMAND_UI(ID_VIEWLOG1, OnUpdateViewlog)
	ON_COMMAND(ID_HIDEWND1, OnHidewnd)
	ON_COMMAND(ID_OPT_DICTIONARY1, OnOptDictionary)
	ON_COMMAND(ID_THREAD_STATISTICS1, OnThreadStatistics)
	ON_UPDATE_COMMAND_UI(ID_THREAD_STATISTICS1, OnUpdateThreadStatistics)
	ON_UPDATE_COMMAND_UI(ID_THREAD_LOG1, OnUpdateThreadLog)
	ON_COMMAND(ID_CUS_LOG11, OnCusLog1)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG11, OnUpdateCusLog1)
	ON_COMMAND(ID_CUS_LOG21, OnCusLog2)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG21, OnUpdateCusLog2)
	ON_COMMAND(ID_CUS_LOG31, OnCusLog3)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG31, OnUpdateCusLog3)
	ON_COMMAND(ID_CUS_LOG41, OnCusLog4)
	ON_UPDATE_COMMAND_UI(ID_CUS_LOG41, OnUpdateCusLog4)
	ON_COMMAND(ID_SEL_CUSTLOG1, OnSelCustlog)
	ON_COMMAND(ID_VIEWLOG1, OnViewlog)
	ON_COMMAND(ID_LEAVEGAME, OnLeavegame)
	ON_UPDATE_COMMAND_UI(ID_LEAVEGAME, OnUpdateLeavegame)
	ON_COMMAND(ID_LEAVEGAME1, OnLeavegame)
	ON_UPDATE_COMMAND_UI(ID_LEAVEGAME1, OnUpdateLeavegame)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INST_START, OnInstStart)
	ON_MESSAGE(WM_INST_STOP, OnInstStop)
	ON_MESSAGE(WM_INST_PAUSE, OnInstPause)
	ON_MESSAGE(WM_INST_RESUME, OnInstResume)
	ON_MESSAGE(WM_INST_STRING, OnInstString)
	ON_MESSAGE(WM_INST_ERROR, OnInstError)
	ON_MESSAGE(WM_INST_PLAYER, OnInstPlayer)
	ON_MESSAGE(WM_INST_PLAYERHP, OnInstPlayerHP)

	ON_MESSAGE(WM_SCH_START, OnScheduleStart)
	ON_MESSAGE(WM_SCH_STOP, OnScheduleStop)
	ON_MESSAGE(WM_SCH_STRING, OnScheduleString)
	ON_MESSAGE(WM_SCH_ERROR, OnScheduleError)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSigView message handlers

BOOL CSigView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}


int CSigView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	g_hViewWnd = m_hWnd;
	if (!m_wndThreads.Create(this, IDC_THREADLIST, NULL, LVS_SHOWSELALWAYS | LVS_SINGLESEL))
		return -1;

	if (!m_wndSchedules.Create(this, IDC_SCHEDULELIST, NULL, LVS_SHOWSELALWAYS | LVS_SINGLESEL))
		return -1;
	
	m_wndThreads.SetImageList(IDB_ITEM);
	m_wndThreads.SetGridLines(TRUE);
	m_wndThreads.SetColumnHeader("Thread,160;Player,220;Status,280;Timer,80, 2");

	m_wndThreads.SetHeaderImageList(IDB_HEADS);
	m_wndThreads.SetHeaderImage(0, 1);
	m_wndThreads.SetHeaderImage(1, 0);
	m_wndThreads.SetHeaderImage(2, 2);	
	m_wndThreads.SetHeaderImage(3, 3);

	m_wndSchedules.SetImageList(IDB_ITEM);
	m_wndSchedules.SetGridLines(TRUE);
	m_wndSchedules.SetColumnHeader("Schedule,160;Status,500");

	m_wndSchedules.SetHeaderImageList(IDB_HEADS);
	m_wndSchedules.SetHeaderImage(0, 1);
	m_wndSchedules.SetHeaderImage(1, 2);

	m_wndThreads.ShowWindow(SW_SHOW);
	m_wndSchedules.ShowWindow(SW_HIDE);

	if (!m_data.Start())
	{
		MessageBox("Failed to start SIG threads!", "Error", MB_ICONSTOP);
		return -1;
	}
	
	return 0;
}

void CSigView::OnSwitchViewType(int nType)
{
	m_nViewType = nType;
	if (nType == 0)
	{
		m_wndSchedules.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
		m_wndSchedules.ShowWindow(SW_HIDE);
		m_wndThreads.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndThreads.SetAllItemStates(RC_ITEM_SELECTED, RC_ITEM_UNSELECTED);
		m_wndThreads.ShowWindow(SW_HIDE);
		m_wndSchedules.ShowWindow(SW_SHOW);
	}	
}

int CSigView::GetViewType() const
{
	return m_nViewType;
}

void CSigView::OnChangeLanguage()
{
	m_wndThreads.SetHeaderText(0, g_bLanguage ? "线程" : "Thread");
	m_wndThreads.SetHeaderText(1, g_bLanguage ? "角色" : "Player");
	m_wndThreads.SetHeaderText(2, g_bLanguage ? "运行状态" : "Running Status");
	m_wndThreads.SetHeaderText(3, g_bLanguage ? "剩时" : "Time Remained");

	m_wndSchedules.SetHeaderText(0, g_bLanguage ? "任务" : "Schedule");
	m_wndSchedules.SetHeaderText(1, g_bLanguage ? "运行状态" : "Running Status");
}

void CSigView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType != SIZE_MINIMIZED)
	{
		m_wndThreads.ResizeToFitParent();
		m_wndSchedules.ResizeToFitParent();
	}
}

void CSigView::OnKill() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (p == NULL)
		return;

	if (p->IsStarted())
	{
		CString sPrompt;
		sPrompt.Format(g_bLanguage ? "即将终止由线程\"%s\"所创建的的Diablo II进程, 确定吗？?" : "Terminating Diablo II process created by thread \"%s\", are you sure?", p->GetName());
		if (MessageBox(sPrompt, "Process Termination", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
			return;
	}	

	p->KillD2Process();
	p->Stop(TRUE);	
	m_wndThreads.SetItemText(nIdx, 2, g_bLanguage ? "已停止" : "Stopped");
	m_wndThreads.SetItemImage(nIdx, 2, IMAGE_STOP);
}

void CSigView::OnUpdateKill(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && p->GetD2HWnd());
}

void CSigView::OnPause() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (p == NULL)
		return;

	if (p->IsPaused())
		 p->Resume();
	else
		 p->Pause();
}

void CSigView::OnUpdatePause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && p->IsStarted());
	pCmdUI->SetCheck(p && p->IsPaused());
}

void CSigView::OnShowd2wnd() 
{
	// TODO: Add your command handler code here
	HideAllD2Wnds(FALSE);
	CSIGInstance* pSel = GetSelectedInstance();
	if (pSel)
		pSel->ShowD2Wnd(TRUE);
}

void CSigView::OnStart() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (p && !p->IsStarted())
		p->Start();
}

void CSigView::OnUpdateStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p != NULL);
	pCmdUI->SetCheck(p && p->IsStarted());
}

void CSigView::OnStop() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (p)
		p->Stop(FALSE);
}

void CSigView::OnUpdateStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && p->IsStarted());
}

void CSigView::OnUpdateViewlog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetSelectedInstance() != NULL);
}

void CSigView::OnNew() 
{
	// TODO: Add your command handler code here
	const CSIGInstance* pInstance = m_data.CreateInstance();
	if (pInstance)
		ListThread(pInstance, &m_wndThreads);
}

void CSigView::OnConfig() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (m_data.EditInstance(p))
		ListThread(p, &m_wndThreads, nIdx);
}

void CSigView::OnUpdateConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && !p->IsStarted());
}

void CSigView::OnDelete() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGInstance* p = GetSelectedInstance(&nIdx);
	if (p == NULL)
		return;

	CString sPrompt;
	sPrompt.Format(g_bLanguage ? "即将删除线程\"%s\", 确定吗？?" : "Deleting thread \"%s\", are you sure?", p->GetData()->sName);
	if (MessageBox(sPrompt, "Delete Thread", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		return;

	if (m_data.DeleteInstance(p))
		m_wndThreads.DeleteItem(nIdx, TRUE);
}

void CSigView::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p != NULL);	
}

void CSigView::OnGlobal() 
{
	// TODO: Add your command handler code here
	m_data.EditGlobalData();
}

LRESULT CSigView::OnInstStart(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndThreads.SetItemImage(nIdx, 2, IMAGE_START);
		m_wndSchedules.SetItemText(nIdx, 2, g_bLanguage ? "运行中" : "Running");
		m_wndThreads.SetItemTextColor(nIdx, 2);
		m_wndThreads.SetItemBkColor(nIdx, 2);
		m_wndThreads.SetItemBkColor(nIdx, 1);
		m_wndThreads.SetItemText(nIdx, 3, "");
	}
	
	return 0;
}

LRESULT CSigView::OnInstStop(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndThreads.SetItemImage(nIdx, 2, IMAGE_STOP);
		m_wndThreads.SetItemText(nIdx, 2, g_bLanguage ? "已停止" : "Stopped");
		m_wndThreads.SetItemText(nIdx, 3, "");
	}

	return 0;
}

LRESULT CSigView::OnInstPause(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndThreads.SetItemImage(nIdx, 2, IMAGE_PAUSE);
		m_wndThreads.SetItemText(nIdx, 2, g_bLanguage ? "暂停" : "Paused");
		m_wndThreads.SetItemText(nIdx, 3, "");
	}
	
	return 0;
}

LRESULT CSigView::OnInstResume(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndThreads.SetItemImage(nIdx, 2, IMAGE_START);
		m_wndThreads.SetItemText(nIdx, 2, g_bLanguage ? "已恢复" : "Resumed");
		m_wndThreads.SetItemText(nIdx, 3, "");
	}

	return 0;
}


LRESULT CSigView::OnInstString(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx == -1)
		return 0;
	
	LPCTHREADMSG pMsg = (LPCTHREADMSG)lParam;
	if (pMsg == NULL)
		return 0;

	static char szTemp[128] = ""; 
	if (pMsg->lpszMsg && pMsg->lpszMsg[0])
	{		
		m_wndThreads.GetItemText(nIdx, 2, szTemp, 127);
		if (::strcmp(pMsg->lpszMsg, szTemp))
			m_wndThreads.SetItemText(nIdx, 2, pMsg->lpszMsg);
	}	

	szTemp[0] = 0;
	if (pMsg->nRemaining > 0)
	{
		int nSeconds = pMsg->nRemaining;
		int nHour = nSeconds / 3600;
		nSeconds %= 3600;
		int nMinute = nSeconds / 60;
		nSeconds %= 60;
		::sprintf(szTemp, "%02d:%02d:%02d", nHour, nMinute, nSeconds);
	}
	
	m_wndThreads.SetItemText(nIdx, 3, szTemp);
	return 0;
}

LRESULT CSigView::OnInstError(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx == -1)
		return 0;

	m_wndThreads.SetItemImage(nIdx, 1, IMAGE_NONE);
	m_wndThreads.SetItemText(nIdx, 1, "");
	m_wndThreads.SetItemImage(nIdx, 2, IMAGE_ERROR);
	m_wndThreads.SetItemText(nIdx, 2, lParam ? (LPCSTR)lParam : "Error Occurred");
	m_wndThreads.SetItemTextColor(nIdx, 2, RGB(192, 0, 0));
	m_wndThreads.SetItemBkColor(nIdx, 2, RGB(255, 192, 192));
	m_wndThreads.SetItemText(nIdx, 3, "");
	return 0;
}

LRESULT CSigView::OnInstPlayerHP(WPARAM wParam, LPARAM lParam)
{
	CSIGInstance* pInstance = (CSIGInstance*)wParam;
	if (pInstance == NULL)
		return 0;

	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx == -1)
		return 0;
	
	LPCPLAYER pPlayer = pInstance->GetPlayer();
	if (pPlayer == NULL)
		return 0;

	WORD wHP = LOWORD(lParam);
	WORD wMaxHP = HIWORD(lParam);
	if (wMaxHP == 0)
		return 0;

	int nPerc = (int)((float)wHP / (float)wMaxHP * 100.0);

	CString str;
	str.Format("%s : %d/%d (%d%%)", pPlayer->szPlayeName, wHP, wMaxHP, nPerc);
	m_wndThreads.SetItemText(nIdx, 1, str);	
	
	if (nPerc < 25)
		m_wndThreads.SetItemBkColor(nIdx, 1, RGB(255, 0, 0));
	else if (nPerc < 50)
		m_wndThreads.SetItemBkColor(nIdx, 1, RGB(255, 192, 192));
	else if (nPerc < 75)
		m_wndThreads.SetItemBkColor(nIdx, 1, RGB(228, 228, 0));
	else
		m_wndThreads.SetItemBkColor(nIdx, 1);
	return 0;
}

LRESULT CSigView::OnInstPlayer(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndThreads.FindItemData(wParam);
	if (nIdx == -1)
		return 0;

	LPCPLAYER pPlayer = (LPCPLAYER)lParam;
	if (pPlayer)
	{
		m_wndThreads.SetItemImage(nIdx, 1, pPlayer->iCharClass + IMAGE_CLASS);
		m_wndThreads.SetItemText(nIdx, 1, pPlayer->szPlayeName);
		m_wndThreads.SetItemBkColor(nIdx, 1);
	}
	else
	{
		m_wndThreads.SetItemImage(nIdx, 1, IMAGE_NONE);
		m_wndThreads.SetItemText(nIdx, 1, "");
		m_wndThreads.SetItemBkColor(nIdx, 1);
	}
	return 0;
}

LRESULT CSigView::OnScheduleStart(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndSchedules.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndSchedules.SetItemImage(nIdx, 1, IMAGE_START);
		m_wndSchedules.SetItemText(nIdx, 1, g_bLanguage ? "运行中" : "Running");
		m_wndSchedules.SetItemTextColor(nIdx, 1);
		m_wndSchedules.SetItemBkColor(nIdx, 1);
	}
	return 0;
}

LRESULT CSigView::OnScheduleStop(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndSchedules.FindItemData(wParam);
	if (nIdx != -1)
	{
		m_wndSchedules.SetItemImage(nIdx, 1, IMAGE_STOP);
		m_wndSchedules.SetItemText(nIdx, 1, g_bLanguage ? "已停止" : "Stopped");
	}
	return 0;
}

LRESULT CSigView::OnScheduleString(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CSigView::OnScheduleError(WPARAM wParam, LPARAM lParam)
{
	int nIdx = m_wndSchedules.FindItemData(wParam);
	if (nIdx == -1)
		return 0;

	m_wndSchedules.SetItemImage(nIdx, 1, IMAGE_ERROR);
	m_wndSchedules.SetItemText(nIdx, 1, lParam ? (LPCSTR)lParam : "Error Occurred");
	m_wndSchedules.SetItemTextColor(nIdx, 1, RGB(192, 0, 0));
	m_wndSchedules.SetItemBkColor(nIdx, 1, RGB(255, 192, 192));
	return 0;
}

void CSigView::OnDestroy() 
{
	m_data.Stop();
	g_hViewWnd = NULL;
	CWnd ::OnDestroy();
}

void CSigView::ListThread(const CSIGInstance *pInstance, CReportCtrl *pList, int nEditIndex)
{
	if (pInstance == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;

	const CSIGThreadData* p = pInstance->GetData();
	ASSERT(p);

	if (nEditIndex != -1)
	{
		pList->SetItemText(nEditIndex, 0, p->sName);
	}
	else
	{
		const int IDX = pList->InsertItemEx(INT_MAX, p->sName, "", g_bLanguage ? "已停止" : "Stopped", "");
		pList->SetItemData(IDX, (DWORD)pInstance);
		pList->SetItemImage(IDX, 0, pInstance->GetMule() ? IMAGE_MULEMODE : IMAGE_NORMAL);
		pList->SetItemImage(IDX, 1, IMAGE_NONE);
		pList->SetItemImage(IDX, 2, IMAGE_STOP);
	}	
}

void CSigView::EnumInstProc(const CSIGInstance *pInstance, LPARAM lParam)
{
	CReportCtrl* pList = (CReportCtrl*)lParam;
	if (pInstance == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;
	ListThread(pInstance, pList);
}

CSIGInstance* CSigView::GetSelectedInstance(int* pIdx) const
{
	if (!::IsWindow(m_wndThreads.m_hWnd))
		return NULL;

	const int IDX = m_wndThreads.GetFirstItem(RC_ITEM_SELECTED);
	if (pIdx)
		*pIdx = IDX;

	if (IDX == -1)
		return NULL;

	return (CSIGInstance*)m_wndThreads.GetItemData(IDX);
}

void CSigView::OnUpdateNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nViewType == 0);
}

void CSigView::OnDblclkThreadList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	NMITEMACTIVATE* p = (NMITEMACTIVATE*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIdx = -1;
	CSIGInstance* pInstance = GetSelectedInstance(&nIdx);
	if (nIdx != -1 && pInstance)
	{
		if (p->iSubItem == 0)
		{
			if (!pInstance->IsStarted())
			{
				if (m_data.EditInstance(pInstance))
					ListThread(pInstance, &m_wndThreads, nIdx);
			}
			else
			{
				pInstance->ShowThreadLog();
			}
		}
		else if (p->iSubItem == 1)
		{
			CStatsDlg dlg(pInstance->GetName(), pInstance->GetStats(), this);
			dlg.DoModal();
		}
		else if (p->iSubItem == 2)
		{
			if (!pInstance->IsStarted())
				pInstance->Start();
			else
			{
				if (pInstance->IsPaused())
					pInstance->Resume();
				else
					pInstance->Pause();
			}
		}
	}

	*pResult = 0;
}

void CSigView::OnDblclkScheduleList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	NMITEMACTIVATE* p = (NMITEMACTIVATE*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nIdx = -1;
	CSIGSchedule* pSchedule = GetSelectedSchedule(&nIdx);
	if (pSchedule == NULL || nIdx == -1)
	{
		*pResult = 0;
		return;
	}

	if (p->iSubItem == 0)
	{
		if (!pSchedule->IsStarted())
		{
			if (m_data.EditSchedule(pSchedule))
				ListSchedule(pSchedule, &m_wndSchedules, nIdx);
		}
	}
	else
	{
		if (pSchedule->IsStarted())
			pSchedule->Stop();
		else
			pSchedule->Start(m_data.GetThreadList());
	}
	*pResult = 0;
}

void CSigView::OnRclickThreadList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE* p = (NMITEMACTIVATE*)pNMHDR;
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->PostMessage(WM_POPUP_MENU, 0, 3);
	*pResult = 0;
}

void CSigView::OnRclickScheduleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE* p = (NMITEMACTIVATE*)pNMHDR;
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->PostMessage(WM_POPUP_MENU, 0, 4);
	*pResult = 0;
}

void CSigView::OnSchNew() 
{
	// TODO: Add your command handler code here
	const CSIGSchedule* p = m_data.CreateSchedule();
	if (p)
		ListSchedule(p, &m_wndSchedules, -1);
}

void CSigView::OnUpdateSchNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nViewType);
}

void CSigView::OnSchConfig() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGSchedule* p = GetSelectedSchedule(&nIdx);
	if (m_data.EditSchedule(p))
		ListSchedule(p, &m_wndSchedules, nIdx);
}

void CSigView::OnUpdateSchConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGSchedule* p = GetSelectedSchedule();
	pCmdUI->Enable(p && !p->IsStarted());
}

void CSigView::OnSchStart() 
{
	// TODO: Add your command handler code here
	CSIGSchedule* p = GetSelectedSchedule();
	if (p && !p->IsStarted())
		p->Start(m_data.GetThreadList());
}

void CSigView::OnUpdateSchStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGSchedule* p = GetSelectedSchedule();
	pCmdUI->Enable(p != NULL);
	pCmdUI->SetCheck(p && p->IsStarted());
}

void CSigView::OnSchStop() 
{
	// TODO: Add your command handler code here
	CSIGSchedule* p = GetSelectedSchedule();
	if (p)
		p->Stop();
}

void CSigView::OnUpdateSchStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGSchedule* p = GetSelectedSchedule();
	pCmdUI->Enable(p && p->IsStarted());
}

void CSigView::OnSchDelete() 
{
	// TODO: Add your command handler code here
	int nIdx = -1;
	CSIGSchedule* p = GetSelectedSchedule(&nIdx);
	if (p == NULL)
		return;

	CString sPrompt;
	sPrompt.Format(g_bLanguage ? "即将删除计划\"%s\", 确定吗？?" : "Deleting schedule \"%s\", are you sure?", p->GetName());
	if (MessageBox(sPrompt, "Delete Schedule", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		return;

	if (m_data.DeleteSchedule(p))
		m_wndSchedules.DeleteItem(nIdx, TRUE);
}

void CSigView::OnUpdateSchDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGSchedule* p = GetSelectedSchedule();
	pCmdUI->Enable(p != NULL);	
}

void CSigView::ListSchedule(const CSIGSchedule *pSchedule, CReportCtrl *pList, int nEditIndex)
{
	if (pSchedule == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;

	if (nEditIndex != -1)
	{
		pList->SetItemText(nEditIndex, 0, pSchedule->GetName());
	}
	else
	{
		const int IDX = pList->InsertItemEx(INT_MAX, pSchedule->GetName(), g_bLanguage ? "已停止" : "Stopped");
		pList->SetItemData(IDX, (DWORD)pSchedule);
		pList->SetItemImage(IDX, 0, IMAGE_SCHEDULE);
		pList->SetItemImage(IDX, 1, IMAGE_STOP);
	}	
}

CSIGSchedule* CSigView::GetSelectedSchedule(int *pIdx) const
{
	if (!::IsWindow(m_wndSchedules.m_hWnd))
		return NULL;

	const int IDX = m_wndSchedules.GetFirstItem(RC_ITEM_SELECTED);
	if (pIdx)
		*pIdx = IDX;

	if (IDX == -1)
		return NULL;

	return (CSIGSchedule*)m_wndSchedules.GetItemData(IDX);
}

void CSigView::EnumSchProc(const CSIGSchedule *pSchedule, LPARAM lParam)
{
	CReportCtrl* pList = (CReportCtrl*)lParam;
	if (pSchedule == NULL || pList == NULL || !::IsWindow(pList->m_hWnd))
		return;
	ListSchedule(pSchedule, pList);
}

void CSigView::OnHidewnd() 
{
	// TODO: Add your command handler code here
	HideAllD2Wnds(TRUE);
}

void CSigView::OnOptDictionary() 
{
	// TODO: Add your command handler code here
	m_data.EditDictionary();
}

void CSigView::HideAllD2Wnds(BOOL bHide) const
{
	for (int i = 0; i < m_wndThreads.GetItemCount(); i++)
	{
		CSIGInstance* p = (CSIGInstance*)m_wndThreads.GetItemData(i);
		if (p == NULL)
			continue;

		if (bHide)
			p->HideD2Wnd();
		else
			p->ShowD2Wnd(FALSE);
	}
}

void CSigView::OnFinishCreation()
{
	const int LOADRES = m_data.LoadData(AfxGetAppEx()->GetAppDir() + SIG_FILE, AfxGetAppEx()->GetAppDir() + DICTIONARY_FILE, AfxGetAppEx()->GetAppDir() + REALM_FILE);

	// List all threads and schedules
	m_data.EnumInstances(EnumInstProc, (LPARAM)&m_wndThreads);
	m_data.EnumSchedules(EnumSchProc, (LPARAM)&m_wndSchedules);


	if (LOADRES == 1)
	{
		// auto run?
		CCmdLine cl(AfxGetApp()->m_lpCmdLine);
		int i = 0;
		CString sSwitch;
		for (i = 0; i < 32; i++)
		{
			if (i)
				sSwitch.Format("-schedule%d", i);
			else
				sSwitch = "-schedule";
		
			CString sVal = cl.GetSafeValue(sSwitch, "");
			CSIGSchedule* p = m_data.FindSchedule(sVal);
			if (p)
				p->Start(m_data.GetThreadList());
		}

		for (i = 0; i < 32; i++)
		{
			if (i)
				sSwitch.Format("-thread%d", i);
			else
				sSwitch = "-thread";

			CString sVal = cl.GetSafeValue(sSwitch, "");
			CSIGInstance* p = m_data.FindThread(sVal);
			if (p)
				p->Start();
		}
	}
	
	if (LOADRES == -1)
	{
		MessageBox(g_bLanguage ?
			"SIG检测到你正在试图使用的数据是由另一个用户创建的(也有可能是你重新安装过你的操作系统)。\n\n出于保护用户Battle.net账号安全的目的，所有已设置的Battle.net账号密码都已被清零，请使用线程设置向导重新设置密码。\n\n对于因此给你带来的不便，我表示歉意。\n\n---- Abin" 
			: "SIG has detected that the data you are accessing were created by another user (or you had reinstalled your operating system). \n\nIn order to protect user Battle.net login information, all previously configured Battle.net login passwords have been discarded. Please use the thread configuration wizard to re-enter your passwords.\n\nI apologize for any inconvenience this may caused you.\n\n---- Abin",
			g_bLanguage? "用户数据变更" : "User Data Changed", MB_ICONEXCLAMATION);
	}

	if (m_wndThreads.GetItemCount() == 0)
	{
		CString sPrompt;
		if (g_bLanguage)
			sPrompt = "你尚未创建任何线程，是否希望立即使用SIG的\"线程设置向导\"来创建新线程？";
		else
			sPrompt = "You have not created any thread, would you like to use the \"Thread Configuration Wizard\" to create new threads now?";

		if (MessageBox(sPrompt, "SIG II", MB_ICONQUESTION | MB_YESNO) == IDYES)
			OnNew();
	}
}

void CSigView::OpenLog(LPCSTR lpszLog) const
{
	if (lpszLog == NULL || lpszLog[0] == 0 || _access(lpszLog, 0))
		return;

	static const char TXTS[] = "log,dat,pwn,d2j,d2l,cfg,ini";

	// get the work directory
	CString sDir = GetDir(lpszLog);
	CString sExt(lpszLog);
	sExt = sExt.Right(3);
	if (sExt.GetLength() == 3 && strstr(TXTS, sExt))
		::ShellExecute(NULL, "open", "notepad.exe", lpszLog, NULL, SW_SHOW);
	else
		::ShellExecute(NULL, "open", lpszLog, NULL, (sDir.IsEmpty() ? NULL : (LPCSTR)sDir) , SW_SHOW);
}

void CSigView::OnThreadLog() 
{
	// TODO: Add your command handler code here
	CSIGInstance* p = GetSelectedInstance();
	if (p)
		p->ShowThreadLog();
}

void CSigView::OnUpdateThreadLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p != NULL);
	
}

void CSigView::OnThreadStatistics() 
{
	// TODO: Add your command handler code here
	const CSIGInstance* p = GetSelectedInstance();
	if (p)
	{
		CStatsDlg dlg(p->GetName(), p->GetStats(), this);
		dlg.DoModal();
	}
}

void CSigView::OnUpdateThreadStatistics(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && p->GetStats()->lStart);
}

void CSigView::OnCusLog1() 
{
	// TODO: Add your command handler code here
	ClickCustLog(0);
}

void CSigView::OnUpdateCusLog1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UpdateCustLogUI(pCmdUI, 0);
}

void CSigView::OnCusLog2() 
{
	// TODO: Add your command handler code here
	ClickCustLog(1);
}

void CSigView::OnUpdateCusLog2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UpdateCustLogUI(pCmdUI, 1);
}

void CSigView::OnCusLog3() 
{
	// TODO: Add your command handler code here
	ClickCustLog(2);
}

void CSigView::OnUpdateCusLog3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UpdateCustLogUI(pCmdUI, 2);
}

void CSigView::OnCusLog4() 
{
	// TODO: Add your command handler code here
	ClickCustLog(3);
}

void CSigView::OnUpdateCusLog4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	UpdateCustLogUI(pCmdUI, 3);
}

void CSigView::UpdateCustLogUI(CCmdUI *pCmdUI, int nIndex)
{
	LPCSTR lpsz = NULL;
	const CSIGInstance* p = GetSelectedInstance();
	if (p)
		lpsz = p->GetCustLog(nIndex);

	if (lpsz == NULL || lpsz[0] == 0)
	{
		pCmdUI->SetCheck(0);
		pCmdUI->SetText(g_bLanguage ? "<空位置>" : "<Empty Slot>");
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->SetText(lpsz);
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(p->GetCurLogIdx() == nIndex);
	}
}

void CSigView::ClickCustLog(int nIndex)
{
	LPCSTR lpsz = NULL;
	CSIGInstance* p = GetSelectedInstance();
	if (p)
		lpsz = p->GetCustLog(nIndex);

	if (lpsz && lpsz[0])
	{
		p->SetCurLogIdx(nIndex);
		OpenLog(lpsz);
	}
}

void CSigView::OnSelCustlog() 
{
	// TODO: Add your command handler code here
	CSIGInstance* p = GetSelectedInstance();
	if (p == NULL)
		return;

	CString sFilter = g_bLanguage ? "Bot Log文件 (*.txt; *.log; *.ini; *.htm; *.html)|*.txt;*.log;*.ini;*.htm;*.html|All Files(*.*)|*.*||" : "Bot Log Files (*.txt; *.log; *.ini; *.htm; *.html)|*.txt;*.log;*.ini;*.htm;*.html|All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, "txt", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, sFilter, this);
	if (dlg.DoModal() != IDOK)
		return;

	p->AddCustomLog(dlg.GetPathName());
	OpenLog(dlg.GetPathName());
}

void CSigView::OnViewlog() 
{
	// TODO: Add your command handler code here
	LPCSTR lpsz = NULL;
	const CSIGInstance* p = GetSelectedInstance();
	if (p)
		lpsz = p->GetCustLog(p->GetCurLogIdx());

	if (lpsz && lpsz[0])
		OpenLog(lpsz);
	else
		OnSelCustlog();
}

void CSigView::OnLeavegame() 
{
	// TODO: Add your command handler code here
	CSIGInstance* p = GetSelectedInstance();
	if(p == NULL || !p->IsInGame())
		return;

	if (p->GetPlayerHPPercent() > 80)
	{
		CString sPrompt;
		sPrompt.Format(g_bLanguage ? "即将强制线程\"%s\"中的角色退出游戏, 确定吗？?" : "Forcing player of thread \"%s\" leave game, are you sure?", p->GetName());
		if (MessageBox(sPrompt, "Process Termination", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
			return;
	}	

	p->LeaveGame();
}

void CSigView::OnUpdateLeavegame(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	const CSIGInstance* p = GetSelectedInstance();
	pCmdUI->Enable(p && p->IsInGame());
}
