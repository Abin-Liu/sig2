// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Sig2.h"

#include "MainFrm.h"
#include "External\\PopupMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_COMMAND(ID_LANG_ENG, OnLangEng)
	ON_UPDATE_COMMAND_UI(ID_LANG_ENG, OnUpdateLangEng)
	ON_COMMAND(ID_LANG_CHS, OnLangChs)
	ON_UPDATE_COMMAND_UI(ID_LANG_CHS, OnUpdateLangChs)
	ON_COMMAND(ID_VIEW_THREADS, OnViewThreads)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THREADS, OnUpdateViewThreads)
	ON_COMMAND(ID_VIEW_SCHEDULES, OnViewSchedules)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCHEDULES, OnUpdateViewSchedules)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_POPUP_SHOWSIG, OnPopupShowsig)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWSIG, OnUpdatePopupShowsig)
	ON_COMMAND(ID_POPUP_HIDESIG, OnPopupHidesig)
	ON_UPDATE_COMMAND_UI(ID_POPUP_HIDESIG, OnUpdatePopupHidesig)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ICON, OnUpdatePanes)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_THREADS, OnUpdatePanes)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCHEDULES, OnUpdatePanes)
	ON_COMMAND(ID_VIEW_THREADS1, OnViewThreads)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THREADS1, OnUpdateViewThreads)
	ON_COMMAND(ID_VIEW_SCHEDULES1, OnViewSchedules)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCHEDULES1, OnUpdateViewSchedules)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAY_NOTIFY, OnTrayNotify)
	ON_MESSAGE(WM_THREAD_COUNT, OnThreadCount)
	ON_MESSAGE(WM_SCHEDULE_COUNT, OnScheduleCount)
	ON_MESSAGE(WM_POPUP_MENU, OnPopup)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_ICON,
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	ID_INDICATOR_THREADS,
	ID_INDICATOR_SCHEDULES
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMainFrame::~CMainFrame()
{
	::DestroyIcon(m_hIcon);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// load menus
	if (!m_menuEng.LoadMenu(IDR_MENU_ENG)
		|| !m_menuChs.LoadMenu(IDR_MENU_CHS)
		|| !m_popupEng.LoadMenu(IDR_POPUP_ENG)
		|| !m_popupChs.LoadMenu(IDR_POPUP_CHS))
	{
		TRACE0("Failed to create menu\n");
		return -1;      // fail to create
	}

	m_menuEng.LoadToolbar(IDR_THREADBAL_ENG);
	m_menuEng.LoadToolbar(IDR_SCHBAL_ENG);
	m_menuEng.LoadToolbar(IDR_TOOLBAR1);
	m_menuChs.LoadToolbar(IDR_THREADBAL_CHS);
	m_menuChs.LoadToolbar(IDR_SCHBAL_CHS);
	m_menuChs.LoadToolbar(IDR_TOOLBAR1);
	m_popupEng.LoadToolbar(IDR_SCHBAL_ENG);
	m_popupEng.LoadToolbar(IDR_TOOLBAR1);
	m_popupChs.LoadToolbar(IDR_SCHBAL_CHS);
	m_popupChs.LoadToolbar(IDR_TOOLBAR1);

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (m_hIcon)
	{
		SetIcon(m_hIcon, TRUE);
		SetIcon(m_hIcon, FALSE);
	}

	SetWindowText(GetAppTitle());

	// create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.GetStatusBarCtrl().SetIcon(0, m_hIcon);
	
	// create tool bar
	if (!CreateToolbar(&m_wndThreadBarEng, IDR_THREADBAL_ENG)
		|| !CreateToolbar(&m_wndThreadBarChs, IDR_THREADBAL_CHS)
		|| !CreateToolbar(&m_wndSchBarEng, IDR_SCHBAL_ENG)
		|| !CreateToolbar(&m_wndSchBarChs, IDR_SCHBAL_CHS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	DWORD dwExStyle = TBSTYLE_EX_DRAWDDARROWS;
	m_wndThreadBarEng.GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)dwExStyle);
	m_wndThreadBarChs.GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)dwExStyle);
	DWORD dwStyle = m_wndThreadBarEng.GetButtonStyle(m_wndThreadBarEng.CommandToIndex(ID_VIEWLOG));
	dwStyle |= TBSTYLE_DROPDOWN;
	m_wndThreadBarEng.SetButtonStyle(m_wndThreadBarEng.CommandToIndex(ID_VIEWLOG), dwStyle);
	m_wndThreadBarChs.SetButtonStyle(m_wndThreadBarChs.CommandToIndex(ID_VIEWLOG1), dwStyle);

	// create system tray icon
	m_tray.Create(this, WM_TRAY_NOTIFY, GetAppTitle(), m_hIcon, IDR_MAINFRAME);

	BOOL bLang = g_bLanguage;
	g_bLanguage = !g_bLanguage;
	OnChangeLanguage(bLang);	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == ID_HELP && nCode)
		AfxGetApp()->WinHelp(0, 0);

	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	CWnd *pWnd;
	int nPos;

	switch (pnmtb->iItem)
	{
	case ID_VIEWLOG:
		pWnd = &m_wndThreadBarEng;
		nPos = 2;
		break;

	case ID_VIEWLOG1:
		pWnd = &m_wndThreadBarChs;
		nPos = 3;
		break;

	default:
		return;
	}
    
	// load and display popup menu
	CMenu* pPopup = g_bLanguage ? m_popupChs.GetSubMenu(1) : m_popupEng.GetSubMenu(1);
	ASSERT(pPopup);
    
	CRect rc;
	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);
    
	pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
							rc.left, rc.bottom, this, &rc);
}

void CMainFrame::OnLangEng() 
{
	// TODO: Add your command handler code here
	OnChangeLanguage(FALSE);
}

void CMainFrame::OnUpdateLangEng(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!g_bLanguage);
}

void CMainFrame::OnLangChs() 
{
	// TODO: Add your command handler code here
	OnChangeLanguage(TRUE);	
}

void CMainFrame::OnUpdateLangChs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bLanguage);
}

void CMainFrame::OnChangeLanguage(BOOL bLanguage)
{
	if (!!g_bLanguage == !!bLanguage)
		return;
	
	g_bLanguage = bLanguage;	
	CMenu* pMenu = g_bLanguage ? &m_menuChs : &m_menuEng;
	SetMenu(pMenu);
	m_hMenuDefault = pMenu->m_hMenu;
	m_wndView.OnChangeLanguage();	
	UpdateToobars();
}

void CMainFrame::OnViewThreads() 
{
	// TODO: Add your command handler code here
	m_wndView.OnSwitchViewType(0);
	UpdateToobars();
}

void CMainFrame::OnUpdateViewThreads(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndView.GetViewType() == 0);
}

void CMainFrame::OnViewSchedules() 
{
	// TODO: Add your command handler code here
	m_wndView.OnSwitchViewType(1);
	UpdateToobars();
}

void CMainFrame::OnUpdateViewSchedules(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	pCmdUI->SetCheck(m_wndView.GetViewType());
}

BOOL CMainFrame::CreateToolbar(CToolBar *pBar, UINT nResID)
{
	ASSERT(pBar);

	if (!pBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
		return FALSE;
	
	return pBar->LoadToolBar(nResID);
}

void CMainFrame::UpdateToobars()
{
	m_wndThreadBarEng.ShowWindow(!g_bLanguage && m_wndView.GetViewType() == 0);
	m_wndThreadBarChs.ShowWindow(g_bLanguage && m_wndView.GetViewType() == 0);
	m_wndSchBarEng.ShowWindow(!g_bLanguage && m_wndView.GetViewType());
	m_wndSchBarChs.ShowWindow(g_bLanguage && m_wndView.GetViewType());
	RecalcLayout();
}

LRESULT CMainFrame::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	if (lParam != WM_RBUTTONUP && lParam != WM_LBUTTONDOWN)
		return 0;

	CWnd* p = this->GetLastActivePopup();
	if (p != this)
	{
		p->FlashWindow(TRUE);
		p->SetForegroundWindow();
		return 0;
	}

	if (lParam == WM_RBUTTONUP)
	{
		// pop up menu
		CMenu* popUp = g_bLanguage ? m_popupChs.GetSubMenu(0) : m_popupEng.GetSubMenu(0);
		if (popUp)
		{
			if (::IsWindowVisible(m_hWnd))
				popUp->SetDefaultItem(ID_POPUP_HIDESIG);
			else
				popUp->SetDefaultItem(ID_POPUP_SHOWSIG);
			
			HWND hWnd = this->GetSafeHwnd();
			::SetForegroundWindow(hWnd);
			PopupMenu(popUp, this);
			::PostMessage(hWnd, WM_NULL, 0, 0);
		}
	}
	else
	{
		if (::IsWindowVisible(m_hWnd))
			OnPopupHidesig();
		else
			OnPopupShowsig();
	}

	return 0;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if (nID == SC_CLOSE)
	{
		OnPopupHidesig();
		return;
	}
	CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnPopupShowsig() 
{
	// TODO: Add your command handler code here
	m_tray.MaximiseFromTray(this);
	m_wndView.HideAllD2Wnds(FALSE);
}

void CMainFrame::OnUpdatePopupShowsig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!::IsWindowVisible(m_hWnd));
}

void CMainFrame::OnPopupHidesig() 
{
	// TODO: Add your command handler code here
	m_wndView.HideAllD2Wnds(TRUE);
	m_tray.MinimiseToTray(this);
}

void CMainFrame::OnUpdatePopupHidesig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsWindowVisible(m_hWnd));
}

void CMainFrame::OnUpdatePanes(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (nType != SIZE_MINIMIZED && ::IsWindow(m_wndStatusBar.m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);
		const int WIDTH = rect.Width();
		int n1 = (int)((double)WIDTH * 0.65);
		int n2 = (int)((double)WIDTH * 0.12);
		m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_ICON, SBPS_NORMAL, 32); 
		m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, n1);      
		m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_THREADS, SBPS_NORMAL, n2);      
		m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_SCHEDULES, SBPS_STRETCH ,0);
	}
}

LRESULT CMainFrame::OnThreadCount(WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(m_wndStatusBar.m_hWnd))
	{
		CString str;
		str.Format(g_bLanguage ? "线程: %d / %d" : "Threads: %d / %d", wParam, lParam);
		m_wndStatusBar.SetPaneText(2, str);
	}
	return 0;
}

LRESULT CMainFrame::OnScheduleCount(WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(m_wndStatusBar.m_hWnd))
	{
		CString str;
		str.Format(g_bLanguage ? "计划: %d / %d" : "Schedules: %d / %d", wParam, lParam);
		m_wndStatusBar.SetPaneText(3, str);
	}
	return 0;
}

void CMainFrame::OnFinishCreation()
{
	m_wndView.OnFinishCreation();
}

LRESULT CMainFrame::OnPopup(WPARAM wParam, LPARAM lParam)
{
	CMenu* pMain = g_bLanguage ? &m_menuChs : &m_menuEng;
	CMenu* pPopup = pMain->GetSubMenu(lParam);
	if (pPopup)
		PopupMenu(pPopup, this);
	return 0;
}
