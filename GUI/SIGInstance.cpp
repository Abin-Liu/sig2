////////////////////////////////////////////////////////////////
// SIGInstance.cpp
//
// SIG control running instance.
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "sig2.h"
#include "SIGInstance.h"
#include "External\\RemoteLib.h"
#include "External\\Ini.h"
#include <time.h>
#include "SIGData.h"

#define D2_WND_CLASS	"Diablo II"
#define AUTO_CONN_INC_BASE	5000
#define AUTO_CONN_RETRY_MAX	5

// a collection of possible error messages
static const LPCTSTR ERROR_MSG[] = {
					_T("D2Loader Error"),
					_T("D2loader.exe - Application Error"),
					_T("Hey guys"),
					_T("Diablo II Error"),
					_T("Diablo II Exception"),
					_T("Diablo II.exe - Application Error"),
					_T("DIABLO2SRV Error"),
					_T("DIABLO2SRV Exception"),
					_T("Ddhelp"),
					_T("Loader Error!"),
					NULL
					};

// define states
enum { ST_NONE = 0,			// Initial state
	   ST_BAN_WAIT,			   
	   ST_CHECK_X_D2WND,	// are there x+ d2 windows in the system?
	   ST_LAUNCH_DIABLO,	// Launch Diablo II
	   ST_CHECK_DIABLO,		// Check wether Diablo II has been launched properly
	   ST_D2LOADED,			// d2 loaded
	   ST_WAIT_BNET,
	   ST_CONNECTBNET,
	   ST_CANCEL_CONNECT,
	   ST_CHECK_CONNECT,
	   ST_LOGIN,			// Login
	   ST_CHECK_LOGIN,		// check login
	   ST_CANCEL_LOGIN,
	   ST_SHOW_CHARLIST,	// show char-selection screen
	   ST_CHECK_CHARLIST,	// check char list
	   ST_SELECT_CHAR,		// Select Character(1-8)
	   ST_CHECK_CHATROOM,	// check if in chat room
	   ST_CHANNEL_STUFF,	// In channel(Join channel, set away)
	   ST_CREATE_GAME,		// Create new game
	   ST_BEFORE_CHECKGAME, // After create action and before checking it
	   ST_CHECK_GAME,		// Check wether game has been created successfully
	   ST_CLOSE_D2,			// Close Diablo II(send WM_CLOSE)
	   ST_WAIT_LAUNCH,		// Wait before launching Diablo II
	   ST_IN_GAME,			// Game created successfully
	   ST_EXIT_GAME,		// Exit game
	   ST_CHECK_EXIT_GAME,	// check if in chat room after exit game
	   ST_PREPARE_CREATE,   // prepare to create new game
	   ST_WAIT_BEFORE_CREATE, // (delay for anti-detect)
	   ST_EXIT_CHANNEL,		// Exit channel
	   ST_CHECK_EXIT_CHANNEL,// check if in char select screen after exit channel
	   ST_EXIT_CHARLIST,	// exit char-select screen
	   ST_CHECK_EXIT_CHARLIST, // check if in login screen after exit char select screen
	   ST_RECONNECT,		// Reconnect to BNet
	   ST_CHECK_RECONNECT,	// check if 
	   ST_CHANNEL_RETRY
 };

void CALLBACK FreeInstance(CSIGInstance*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

CSIGInstance::CSIGInstance(LPCGLOBALDATA pGlobal, const CStringArrayEx* pDictionary, BOOL bMuleMode)
: m_pGlobal(pGlobal), m_bMuleMode(bMuleMode)
{
	ASSERT(pGlobal && pDictionary);
	m_dictionary.SetNameArray(pDictionary);
	m_ticker.Clear();
	m_hD2Wnd = NULL;
	m_dwD2ProcessID = 0;
	::memset(&m_wp, 0, sizeof(m_wp));
	m_dwConnectRetry = 0;
	m_bForceD2OnTop = FALSE;
	m_bPlacementChanged = FALSE;
	m_bGameJoined = FALSE;
	m_dwJoinFail = 0;
	m_dwLoginFail = 0;
	m_nGames = 0;
	m_nTimeLimit = -1;
	m_nGameLimit = -1;
	::memset(&m_player, 0, sizeof(m_player));
	m_dwHP = 0;
	m_dwMaxHP = 0;
	m_nNextGameCrTime = 0;
	m_nBannedTest = 0;
	m_nRealmDownTest = 0;
	m_bFirstGame = TRUE;
	::memset(&m_stats, 0, sizeof(m_stats));
	UpdateLogPath();
}

CSIGInstance::~CSIGInstance()
{
	Stop(FALSE);
}

BOOL CSIGInstance::LoadData(CStreamBuffer& sb, LPCSTR lpszPassword)
{
	m_ticker.Clear();
	m_data.LoadData(sb, lpszPassword);
	sb >> m_bMuleMode;
	if (m_bMuleMode)
		m_mule.LoadMuleData(sb, lpszPassword);
	m_dictionary.SetRule(&m_data.nameRule);
	UpdateLogPath();
	return TRUE;
}

BOOL CSIGInstance::SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const
{
	m_data.SaveData(sb, lpszPassword);
	sb << m_bMuleMode;
	if (m_bMuleMode)
		m_mule.SaveMuleData(sb, lpszPassword);

	return TRUE;
}

BOOL CSIGInstance::Start(int nTimeLimit, int nGameLimit)
{
	if (IsStarted())
		return FALSE;	

	if (!Verify())
	{
		PostError("本线程中某些设置不正确", "Some setting of this thread are invalid");
		return FALSE;
	}

	m_bFirstGame = TRUE;
	::SendMessage(g_hViewWnd, WM_INST_START, (WPARAM)this, 0);
	m_logger.Flush();
	m_logger.AddLineBreak();
	ShowInfo("线程\"%s\"启动", "Thread \"%s\" started", m_data.sName);
	::memset(&m_player, 0, sizeof(m_player));
	m_dwHP = 0;
	m_dwMaxHP = 0;
	m_nTimeLimit = nTimeLimit;
	m_nGameLimit = nGameLimit;
	m_nNextGameCrTime = 0;	
	m_dwConnectRetry = 0;
	m_bForceD2OnTop = FALSE;
	m_bPlacementChanged = FALSE;
	m_bGameJoined = FALSE;
	m_dwJoinFail = 0;
	m_dwLoginFail = 0;
	m_nBannedTest = 0;
	m_nRealmDownTest = 0;
	m_mule.Initialize();
	m_ticker.Clear();
	KillD2Process();

	if (m_bMuleMode && m_mule.GetNextLogin(m_data.login) == MULE_NO_MORE)
	{
		Stop(FALSE);
		return FALSE;
	}
	
	::memset(&m_stats, 0, sizeof(m_stats));
	::time(&m_stats.lStart);

	m_ticker.Update(ST_CHECK_X_D2WND);
	m_nGames = 0;
	return TRUE;
}

BOOL CSIGInstance::Pause()
{
	if (!IsStarted() || m_ticker.IsPaused())
		return FALSE;
	m_ticker.Pause();
	::SendMessage(g_hViewWnd, WM_INST_PAUSE, (WPARAM)this, 0);
	return TRUE;
}

BOOL CSIGInstance::Resume()
{
	if (!IsPaused())
		return FALSE;

	m_ticker.Resume();
	::SendMessage(g_hViewWnd, WM_INST_RESUME, (WPARAM)this, 0);
	ShowInfo(m_sInfoBeforePaused, m_sInfoBeforePaused);
	return TRUE;
}

BOOL CSIGInstance::Stop(BOOL bKillD2)
{
	if (IsStarted())
	{
		m_stats.lStop = ::time(NULL);
		OnGameLeave();
		m_ticker.Clear();
		if (g_pCurrentLaunchingInst == this)
			g_pCurrentLaunchingInst = NULL;

		if (bKillD2)
			KillD2Process(m_hD2Wnd, m_dwD2ProcessID);
		else
			ShowD2Wnd(TRUE);

		ShowInfo("线程\"%s\"终止", "Thread \"%s\" stopped", m_data.sName);
		m_logger.AddLineBreak();
		m_logger.Flush();
	}
	
	::SendMessage(g_hViewWnd, WM_INST_STOP, (WPARAM)this, 0);
	return TRUE;
}

BOOL CSIGInstance::IsStarted() const
{
	return m_ticker.GetState() != ST_NONE;
}

BOOL CSIGInstance::IsPaused() const
{
	return IsStarted() && m_ticker.IsPaused();
}

void CSIGInstance::ShowInfo(LPCSTR lpszChsFmt, LPCSTR lpszEngFmt, ...)
{
	static char szString[512] = "";
	static THREADMSG msg = { 0 };

	LPCSTR lpszStr = g_bLanguage ? lpszChsFmt : lpszEngFmt;	
	if (lpszStr && lpszStr[0])
	{		
		va_list args;
		va_start(args, lpszEngFmt);
		_vsnprintf(szString, 511, lpszStr, args);
		va_end(args);	
		lpszStr = szString;		
	}	

	m_sInfoBeforePaused = szString;
	msg.lpszMsg = lpszStr;
	msg.nRemaining = 0;
	::SendMessage(g_hViewWnd, WM_INST_STRING, (WPARAM)this, (LPARAM)&msg);	
	CString str(lpszStr);
	if (str.Right(3) != "...")
		m_logger.Log(lpszStr);
}

BOOL CSIGInstance::IsD2Wnd(HWND hWnd)
{
	if (hWnd == NULL || !::IsWindow(hWnd))
		return FALSE;

	char szClass[20] = "";
	::GetClassName(hWnd, szClass, 19);
	return ::stricmp(szClass, D2_WND_CLASS) == 0;
}

void CSIGInstance::KillD2Process()
{
	if (m_hD2Wnd)
	{
		ShowInfo("正在关闭Diablo II", "Closing Diablo II");
		KillD2Process(m_hD2Wnd, m_dwD2ProcessID);
	}	
	m_hD2Wnd = NULL;
	m_dwD2ProcessID = 0;
}

void CSIGInstance::KillD2Process(HWND hD2Wnd, DWORD dwProcessID)
{
	if (dwProcessID == 0)
		::GetWindowThreadProcessId(hD2Wnd, &dwProcessID);

	if (IsD2Wnd(hD2Wnd))
	{
		DWORD dwDummy;
		::SendMessageTimeout(hD2Wnd, WM_CLOSE, 0, 0, SMTO_BLOCK, 5000, &dwDummy);
	}

	if (dwProcessID)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID); 
		if (hProcess) // validate process handle
		{
			::TerminateProcess(hProcess, 0); // terminate the process
			::CloseHandle(hProcess); // close the handle
		}
	}	
}

void CSIGInstance::CheckAndKillD2s()
{
	if (m_pGlobal == NULL || m_pGlobal->nMaxD2Process <= 0)
		return;

	ShowInfo("正在检查系统进程", "Verifying system processes");
	CArrayEx<HWND, HWND> aD2Wnds;
	::EnumWindows(EnumWindowProc, (LPARAM)&aD2Wnds);
	if (aD2Wnds.GetSize() + 1 > m_pGlobal->nMaxD2Process)
	{
		ShowInfo("系统中D2进程将要达到%d个 (限制值为%d个)", "System D2 process number reaching %d (Maximum is %d)", aD2Wnds.GetSize() + 1, m_pGlobal->nMaxD2Process);
		for (int i = 0; i < aD2Wnds.GetSize(); i++)
			KillD2Process(aD2Wnds[i]);
	}
}

BOOL CSIGInstance::EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	CArrayEx<HWND, HWND>* pList = (CArrayEx<HWND, HWND>*)lParam;
	if (pList == NULL)
		return FALSE;

	if (IsD2Wnd(hWnd))
		pList->Add(hWnd);

	return TRUE;
}

void CSIGInstance::PostError(LPCSTR lpszChsFmt, LPCSTR lpszEngFmt, ...)
{
	m_ticker.Clear();
	Stop(TRUE);

	LPCSTR lpszFmt = g_bLanguage ? lpszChsFmt : lpszEngFmt;
	if (lpszFmt == NULL || lpszFmt[0] == 0)
	{
		::SendMessage(g_hViewWnd, WM_INST_ERROR, (WPARAM)this, (LPARAM)lpszFmt);	
	}
	else
	{
		char szString[512] = "";
		va_list args;
		va_start(args, lpszEngFmt);
		_vsnprintf(szString, 511, lpszFmt, args);
		va_end(args);
		::SendMessage(g_hViewWnd, WM_INST_ERROR, (WPARAM)this, (LPARAM)szString);	
	}	
}

BOOL CSIGInstance::LaunchDiablo()
{
	ShowInfo("正在启动Diablo II", "Starting Diablo II");

	const CString D2DIR = GetDir(m_data.sD2Executable);
	::DeleteFile(D2DIR + "\\bncache.dat");
	
	CRealmInfo::ForceDefaultRealm(m_data.login.sBnetIP, m_data.login.sSubRealm);
	CRealmInfo::SetDefaultAccount(m_data.login.sAccount);
	CRealmInfo::DisableSkipToOpen(TRUE);	
	
	CString sParamLine = m_data.sParamLine;
	sParamLine += " -w -skiptobnet -nonotify";

	ShowInfo("创建进程: %s %s", "Creating process: %s %s", m_data.sD2Executable, sParamLine);

	// create d2 snapshot
	m_aD2Snapshot.RemoveAll();
	::EnumWindows(EnumWindowProc, (LPARAM)&m_aD2Snapshot);
	HINSTANCE h = ::ShellExecute(NULL, "open", m_data.sD2Executable, sParamLine, D2DIR, SW_MINIMIZE);
	if ((long)h > 32L)
		return TRUE;

	PostError("无法启动Diablo II", "Failed to launch Diablo II");
	return FALSE;
}

BOOL CSIGInstance::VerifyD2Wnd()
{
	m_hD2Wnd = NULL;
	m_dwD2ProcessID = 0;
	m_spy.SetD2Wnd(NULL);
	m_auto.SetTargetHWnd(NULL);

	CArrayEx<HWND, HWND> aNewSnapshot;
	::EnumWindows(EnumWindowProc, (LPARAM)&aNewSnapshot);
	for (int i = 0; i < aNewSnapshot.GetSize(); i++)
	{
		if (aNewSnapshot[i] && m_aD2Snapshot.Find(aNewSnapshot[i]) == -1)
		{
			m_hD2Wnd = aNewSnapshot[i];
			::GetWindowThreadProcessId(m_hD2Wnd, &m_dwD2ProcessID);
			break;
		}
	}

	if (!m_dwD2ProcessID)
		return FALSE;

	m_spy.SetD2Wnd(m_hD2Wnd);
	m_auto.SetTargetHWnd(m_hD2Wnd);
	ShowInfo("游戏启动正常: HWND 0x%08X, Process 0x%08X", "Game started: HWND 0x%08X, Process 0x%08X", m_hD2Wnd, m_dwD2ProcessID);
	return TRUE;
}

BOOL CSIGInstance::LoadPlugins()
{
	if (m_data.bAntiIdle)
	{
		ShowInfo("正在加载\"AntiIdle.dll\"", "Loading \"AntiIdle.dll\"");
		HMODULE hModule = RemoteLoadLibrary(m_hD2Wnd, AfxGetAppEx()->GetAppDir() + "\\AntiIdle.dll");
		if (hModule == NULL)
			ShowInfo("加载\"AntiIdle.dll\"失败", "Failed to load \"AntiIdle.dll\"");
	}

	LPCSTR szLoadDll = NULL;

	if (m_data.nHackType == HACK_D2HACKIT)
	{
		ShowInfo("正在加载D2Hackit", "Loading D2Hackit");
		szLoadDll = m_data.sD2HackitDll;	
		const CString D2HINI = GetDir(szLoadDll) + "\\D2Hackit.ini";
		CIni ini(D2HINI);
		ini.WriteString("Misc", "Autoload", m_data.sModules);
	}
	else if (m_data.nHackType == HACK_D2JSP)
	{
		ShowInfo("正在加载D2JSP", "Loading D2JSP");
		szLoadDll = m_data.sD2JspDll;
		const CString SCRIPTDIR = GetDir(szLoadDll) + "\\scripts\\bots";
		::CopyFile(SCRIPTDIR + "\\autostarter.d2j", SCRIPTDIR + "\\autostarter.bak", FALSE);
		::CopyFile(SCRIPTDIR + "\\starter.d2j", SCRIPTDIR + "\\starter.bak", FALSE);
		::DeleteFile(SCRIPTDIR + "\\autostarter.d2j");
		::DeleteFile(SCRIPTDIR + "\\starter.d2j");

		if (m_data.bAllowModifyScript)
			UpdateAutoloadScripts(GetDir(szLoadDll) + "\\scripts\\default.d2j");
	}
	else
	{
		return TRUE;
	}

	// cleanup first
	HMODULE hModule = RemoteGetModuleHandle(m_hD2Wnd, "D2Hackit.dll");
	if (hModule)
		RemoteFreeLibrary(m_hD2Wnd, hModule);

	hModule = RemoteGetModuleHandle(m_hD2Wnd, "D2Jsp.dll");
	if (hModule)
		RemoteFreeLibrary(m_hD2Wnd, hModule);

	hModule = RemoteLoadLibrary(m_hD2Wnd, szLoadDll, INFINITE);
	if (hModule == NULL)
	{
		PostError("加载\"%s\"失败", "Failed to load \"%s\"", szLoadDll);
		return FALSE;
	}

	return TRUE;
}

BOOL CSIGInstance::ShowD2Wnd(BOOL bBringToTop)
{
	m_bForceD2OnTop = FALSE;
	if (m_hD2Wnd == NULL)
		return FALSE;

	if (m_bPlacementChanged)
	{
		::SetWindowPlacement(m_hD2Wnd, &m_wp);
		m_bPlacementChanged = FALSE;
	}

	if (bBringToTop)
	{
		::ShowWindow(m_hD2Wnd, SW_MINIMIZE);
		::ShowWindow(m_hD2Wnd, SW_RESTORE);
		::SetWindowPos(m_hD2Wnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		::ShowWindow(m_hD2Wnd, SW_SHOWNOACTIVATE);
		::SetWindowPos(m_hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	return TRUE;
}

void CSIGInstance::OnGameJoin()
{
	m_nBannedTest = 0;
	m_stats.nGameOKs++;
	m_stats.nCurGameLen = 0;
	m_dwHP = 0;
	m_dwMaxHP = 0;
	m_playerStats.SetD2Wnd(m_hD2Wnd);
	m_nGames++;
	m_bGameJoined = TRUE;
	m_dwJoinFail = 0;
	m_ticker.Update(ST_IN_GAME);
	m_spy.UpdatePlayerData();

	int nLimit = GetInGameLimit();
	if (nLimit > 0)
		ShowInfo("加入游戏成功，停留时限%d秒", "Game joined successfully, Maximum stay %d seconds", nLimit);
	else
		ShowInfo("加入游戏成功", "Game joined successfully");

	LPCPLAYER pPlayer = m_spy.GetPlayer();
	if (pPlayer)
		::memcpy(&m_player, pPlayer, sizeof(m_player));

	::SendMessage(g_hViewWnd, WM_INST_PLAYER, (WPARAM)this, (LPARAM)&m_player);
}

void CSIGInstance::OnGameLeave()
{
	m_bFirstGame = FALSE;
	if (!m_stats.nCurGameLen)
		return;	

	if (m_stats.nCurGameLen < m_stats.nMinGame)
		m_stats.nMinGame = m_stats.nCurGameLen;

	m_dwHP = 0;
	m_dwMaxHP = 0;
	if (!m_bGameJoined)
		return;

	if (m_bMuleMode)
		m_nNextGameCrTime = 0;
	else
		m_nNextGameCrTime = m_data.nMin2Games - m_stats.nCurGameLen;

	m_playerStats.SetD2Wnd(NULL);
	m_bGameJoined = FALSE;
	m_dwJoinFail = 0;	
	ShowInfo("游戏已退出", "Game exit");
	::SendMessage(g_hViewWnd, WM_INST_PLAYER, (WPARAM)this, 0);
	if (m_nGameLimit > 0 && m_nGames >= m_nGameLimit)
		Stop(TRUE);
	else
		m_ticker.Update(ST_CHECK_EXIT_GAME);
}

void CSIGInstance::MoveD2WndOut()
{
	if (m_hD2Wnd == NULL || (::IsWindowVisible(m_hD2Wnd) && !::IsIconic(m_hD2Wnd)))
		return;
	
	m_bPlacementChanged = TRUE;
	WINDOWPLACEMENT wp;
	memcpy(&wp, &m_wp, sizeof(WINDOWPLACEMENT));
	wp.rcNormalPosition.left = 800;
	wp.rcNormalPosition.top = 3000;
	wp.rcNormalPosition.right = 801;
	wp.rcNormalPosition.bottom = 3001;
	::SetWindowPlacement(m_hD2Wnd, &wp);

	if (::IsIconic(m_hD2Wnd) || !::IsWindowVisible(m_hD2Wnd))
	{
		::ShowWindow(m_hD2Wnd, SW_SHOWNOACTIVATE);				
		::SetWindowPos(m_hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	m_bForceD2OnTop = TRUE;
}

void CSIGInstance::MoveD2WndIn()
{
	m_bForceD2OnTop = FALSE;

	if (m_hD2Wnd == NULL)
		return;	

	if (::IsWindowVisible(m_hD2Wnd) && !::IsIconic(m_hD2Wnd))
	{
		CRect rect;
		::GetWindowRect(m_hD2Wnd, &rect);
		if (rect.Width() > 500 && rect.Height() > 400)
			return;
	}
	
	m_bPlacementChanged = FALSE;
	::ShowWindow(m_hD2Wnd, SW_MINIMIZE);
	::SetWindowPlacement(m_hD2Wnd, &m_wp);
	::ShowWindow(m_hD2Wnd, AfxGetMainWnd()->IsWindowVisible() ? SW_MINIMIZE : SW_HIDE);
}

void CSIGInstance::CheckCharList()
{
	int nNum = m_spy.GetCharCount();
	if (nNum <= 0)
	{
		if (!IsWaiting("正在等待角色列表", "Waiting for player list"))
		{
			m_nRealmDownTest++;
			MoveD2WndIn();
			ShowInfo("角色列表超时，准备重试", "Player list timeout, retrying");
			if (m_nRealmDownTest < 3)
			{
				m_auto.SendEscape();
				ShowInfo("正在退出角色选择界面", "Exiting player selection");				
				m_ticker.Update(ST_RECONNECT, 8000);
			}
			else
			{
				m_nRealmDownTest = 0;
				OnRealmDown();
			}
		}

		return;
	}	

	m_nRealmDownTest = 0;
	ShowInfo("角色列表接收成功，总共%d个角色", "Player list received, %d players", nNum);
	MoveD2WndIn();

	int nPos = m_data.login.nCharIdx;

	if (nPos >= nNum)
	{
		ShowInfo("无效角色位置(第%d个 共%d个)", "Invalid player position (%d of %d)", nPos + 1, nNum);
		if (m_bMuleMode)
			m_ticker.Update(ST_EXIT_CHARLIST);
		else
			m_ticker.Update(ST_CLOSE_D2, 5000);
	}
	else
	{
		m_ticker.Update(ST_SELECT_CHAR);
	}
}

void CSIGInstance::OnWndTick()
{
	if (!m_bForceD2OnTop || m_ticker.GetState() == ST_NONE || m_ticker.IsPaused())
		return;

	if (::IsIconic(m_hD2Wnd) || !::IsWindowVisible(m_hD2Wnd))
	{
		::ShowWindow(m_hD2Wnd, SW_SHOWNOACTIVATE);				
		::SetWindowPos(m_hD2Wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}

void CSIGInstance::CreateGame()
{
	if (!AfxGetMainWnd()->IsWindowVisible())
		::ShowWindow(m_hD2Wnd, SW_HIDE);

	if (!m_spy.IsInChatRoom())
	{
		m_ticker.Update(ST_CHANNEL_RETRY, 15000);
		return;
	}

	if (!m_ticker.IsTickEnded())
		return;

	CString sName = m_dictionary.GetGameName();
	CString sPwd = m_dictionary.GetGamePwd();

	m_stats.nGames++;
	ShowInfo("正在创建游戏\"%s", "Creating game \"%s\"", sName);
	m_auto.CreateGame(sName, sPwd, m_data.nDifficulty, m_pGlobal->nButtonDelay * 1000, 200);
	m_ticker.Update(ST_BEFORE_CHECKGAME, 7000);
}

void CSIGInstance::CheckGame()
{
	int nNum = (int)m_spy.GetCreationQueue();
	if (nNum)
		ShowInfo("创建游戏队列 %d...", "Game in queue %d...", nNum);
	else
		IsWaiting("正在检查游戏状态", "Checking game status");

	if (m_bGameJoined || m_ticker.GetElapsed() < 4000)
		return;

	// not yet joined, check if we got some problem
	BOOL bAbort = FALSE;
	DWORD dwExtraDelay = 5000;
	switch (m_spy.GetCreationResult())
	{
	case CREATE_NAME_INVALID:
		ShowInfo("非法游戏名", "Invalid game name");
		bAbort = TRUE;
		break;

	case CREATE_NAME_EXIST:
		ShowInfo("游戏名已存在", "Game already exists");
		bAbort = TRUE;
		break;

	case CREATE_FAILED:
		ShowInfo("加入游戏失败", "Failed to join game");
		bAbort = TRUE;
		break;

	case CREATE_SERVER_DOWN:
		ShowInfo("服务器停用", "Server down");
		bAbort = TRUE;
		break;

	case CREATE_NOT_EXIST:
		ShowInfo("游戏不存在", "Game not exists");
		m_nBannedTest++;
		if (m_nBannedTest >= 2)
		{
			OnCDKeyBanned();
			return;
		}
		bAbort = TRUE;
		break;

	default:
		dwExtraDelay = 10000;
		break;
	}

	if (bAbort || m_ticker.IsTickEnded())
	{
		if (m_dwJoinFail < (DWORD)3)
		{
			// not yet exceed maximum retry counter, so try again
			m_auto.CancelQueue();
			m_dwJoinFail++;
			m_ticker.Update(ST_CREATE_GAME, dwExtraDelay);
		}
		else
		{
			// completely failed
			if (!m_bMuleMode)
			{
				// not running mule mode, just shut down D2					
				ShowInfo("进入游戏失败，准备关闭Diablo II", "Failed to join game, prepare to close Diablo II");
				m_ticker.Update(ST_CLOSE_D2, 2000);
			}
			else
			{
				ShowInfo("该角色进入游戏失败，准备访问下一个角色", "Failed to join game, prepare for next player");
				m_ticker.Update(ST_EXIT_CHANNEL, 10000);
				
			}
		}
	}
}

void CSIGInstance::OnInstanceTick()
{
	if (m_ticker.GetState() == ST_NONE)
		return;

	m_ticker.TickIt(INSTANCE_TICK);
	m_stats.nTotalRunning++;

	if (m_nNextGameCrTime > 0)
		m_nNextGameCrTime--;

	if (m_bGameJoined)
	{
		DWORD dwHP = m_playerStats.GetStats(D2PS_HP);
		DWORD dwMaxHP = m_playerStats.GetStats(D2PS_MAXHP);
		if (dwMaxHP)
		{
			if (dwHP != m_dwHP || dwMaxHP != m_dwMaxHP)
			{
				m_dwHP = dwHP;
				m_dwMaxHP = dwMaxHP;
				::SendMessage(g_hViewWnd, WM_INST_PLAYERHP, (WPARAM)this, MAKELPARAM((WORD)dwHP, (WORD)dwMaxHP));
			}
		}
	}

	if (m_ticker.IsPaused())
		return;

	if (m_nTimeLimit > 0)
		m_nTimeLimit--;
		
	if (m_nTimeLimit == 0)
	{
		Stop(TRUE);
		return;
	}

	if (!VerifyGame())
	{
		if (m_bGameJoined)
			OnGameLeave();
		m_ticker.Update(ST_CLOSE_D2);
		ShowInfo("游戏意外终止, 准备重试", "Game terminated unexpectedly, prepare to retry");
		m_stats.nCrashes++;
	}
	else
	{
		const BOOL INGAME = m_spy.IsInGame();
		if (m_bGameJoined && !INGAME)
			OnGameLeave();
		else if (!m_bGameJoined && INGAME)
			OnGameJoin();
	}

	static int nNum = 0;

	switch (m_ticker.GetState())
	{
	case ST_BAN_WAIT:
		if (!IsWaiting("CDKey Banned或Realm Down，准备重试", "CDKey Banned or Realm Down, Prepare to retry"))
			m_ticker.Update(ST_CHECK_X_D2WND);
		break;

	case ST_CLOSE_D2:		
		
		if (!IsWaiting("准备关闭Diablo II", "Prepare to close Diablo II"))
		{
			KillD2Process();
			m_ticker.Update(ST_CHECK_X_D2WND, 6000);
		}
		break;

	case ST_CHECK_X_D2WND: // are there x+ d2 windows in the system?

		if (!IsWaiting("正在检查系统进程", "Checking system processes"))
		{
			CheckAndKillD2s();
			m_ticker.Update(ST_WAIT_LAUNCH, 5000);
		}
	
		break;

	case ST_WAIT_LAUNCH:
		if (!IsWaiting("准备启动Diablo II", "Prepare to starting Diablo II"))
			m_ticker.Update(ST_LAUNCH_DIABLO);
		break;

	case ST_LAUNCH_DIABLO: // Launch Diablo II

		if (g_pCurrentLaunchingInst == NULL || g_pCurrentLaunchingInst == this)
		{
			g_pCurrentLaunchingInst = this;			

			if (LaunchDiablo())
				m_ticker.Update(ST_CHECK_DIABLO, m_pGlobal->nLaunchDelay * 1000);
		}
		else
		{			
			ShowInfo("正在等待另一个SIG线程完成启动Diablo II的过程", "Waiting for another SIG thread to complete launching Diablo II");
		}
		
		break;
		
	case ST_CHECK_DIABLO: // Check wether Diablo II has been launched properly
		if (IsWaiting("正在在检查游戏启动状态", "Verifying game states"))
		{
			if (VerifyD2Wnd())
			{
				if (g_pCurrentLaunchingInst == this)
					g_pCurrentLaunchingInst = NULL;	
				
				// save window placement
				memset(&m_wp, 0, sizeof(WINDOWPLACEMENT));
 				m_wp.length = sizeof(WINDOWPLACEMENT); 
 				::GetWindowPlacement(m_hD2Wnd, &m_wp);
				m_ticker.Update(ST_D2LOADED, 5000);				
			}			
		}
		else
		{
			PostError("无法启动Diablo II", "Failed to launch Diablo II");
		}

		break;

	case ST_D2LOADED:

		CheckAndHide();
		if (!IsWaiting("等待游戏进程稳定化", "Waiting for game process stabilizing"))
		{
			if (LoadPlugins())
				m_ticker.Update(ST_WAIT_BNET, 8000);
		}		
		
		break;

	case ST_WAIT_BNET:
		CheckAndHide();
		if (IsWaiting("正在等待Battle.net连接", "Waiting for Battle.net connection")
			|| m_spy.IsBNetConnectSuccessful())
			m_ticker.Update(ST_CANCEL_CONNECT, m_pGlobal->nConnectDelay * 1000);
		break;

	case ST_CONNECTBNET:
		CheckAndHide();
		if (!IsWaiting("正在连接到Battle.net", "Connecting to Battle.net"))
		{
			m_auto.ConnectToBNet();	
			m_ticker.Update(ST_CANCEL_CONNECT, m_pGlobal->nConnectDelay * 1000 + m_dwConnectRetry * AUTO_CONN_INC_BASE);
		}
		break;

	case ST_CANCEL_CONNECT:		
		CheckAndHide();
		if (!IsWaiting("正在检查Battle.net连接", "Verifying Battle.net connection"))
		{	
			m_auto.CancelConnect();
			m_ticker.Update(ST_CHECK_CONNECT, 3000);
		}
		break;

	case ST_CHECK_CONNECT:
		CheckAndHide();
		if (m_spy.IsBNetConnectSuccessful())
		{
			ShowInfo("Battle.net连接成功", "Connected to Battle.net successfully");			
			m_ticker.Update(ST_LOGIN);
		}
		else
		{			
			ShowInfo("Battle.net连接超时，准备重试", "Battle.net connection timeout, retrying");			

			if (m_dwConnectRetry < AUTO_CONN_RETRY_MAX)
			{
				m_dwConnectRetry++;
				m_ticker.Update(ST_CONNECTBNET);
			}
			else
			{
				m_dwConnectRetry = 0;
				m_ticker.Update(ST_CLOSE_D2);
			}
		}

		break;

	case ST_LOGIN:
		CheckAndHide();
		if (!IsWaiting("准备登录Battle.net", "Prepare to login to Battle.net"))
		{
			m_stats.nLogins++;
			ShowInfo("正在登录账号\"%s\"", "Login account \"%s\"", m_data.login.sAccount);
			m_auto.Login(m_data.login.sAccount, m_data.login.sPassword);
			m_ticker.Update(ST_CHECK_LOGIN, 4000 + m_pGlobal->nLoginDelay * 1000);
			MoveD2WndOut(); // activate d2 window
		}
		
		break;

	case ST_CHECK_LOGIN:
		CheckAndHide();
		if (IsWaiting("正在核实登录情况", "Verifying login result"))
		{
			if (m_spy.IsBNetLoginSuccessful())
			{
				//MoveD2WndIn();
				m_stats.nLoginOKs++;
				ShowInfo("登录成功", "Login successfully");
				m_dwConnectRetry = 0;			
				m_ticker.Update(ST_SHOW_CHARLIST);
			}
		}
		else
		{
			MoveD2WndIn();
			m_dwLoginFail++;
			m_auto.CancelConnect();
			ShowInfo("登录Battle.net超时,准备重试", "Connection timeout, retrying");
			m_ticker.Update(ST_CANCEL_LOGIN, 3000);
		}

		break;

	case ST_CANCEL_LOGIN:
		CheckAndHide();
		if (!IsWaiting())
		{
			if (m_dwLoginFail < 3)
			{
				if (m_spy.IsBNetConnectSuccessful())
					m_ticker.Update(ST_LOGIN);
				else
					m_ticker.Update(ST_CONNECTBNET);
			}
			else
			{
				if (m_bMuleMode)
				{	
					if (m_mule.GetNextLogin(m_data.login) == MULE_NO_MORE)
					{
						Stop(TRUE);
					}
					else
					{
						ShowInfo("当前账号访问失败,准备登录下一个账号", "Current account login failed, prepare for next account");
						m_ticker.Update(ST_CLOSE_D2);
					}
				}
				else
				{
					m_ticker.Update(ST_CLOSE_D2);
				}
			}
		}
		
		break;

	case ST_SHOW_CHARLIST:		

		ShowInfo("正在等待角色列表", "Waiting for player list");		
		MoveD2WndOut();
		m_ticker.Update(ST_CHECK_CHARLIST, m_pGlobal->nLoginDelay * 1000);
		break;

	case ST_CHECK_CHARLIST:
		
		CheckCharList();
		break;

	case ST_SELECT_CHAR:
		CheckAndHide();
		nNum = m_data.login.nCharIdx;
		ShowInfo("正在选取角色(位置%d)", "Selecting player (%d)", nNum + 1);
		m_auto.SelectChar(nNum);		
		m_ticker.Update(ST_CHECK_CHATROOM, m_pGlobal->nCharSelDelay * 1000);		
		break;

	case ST_CHECK_CHATROOM:
		CheckAndHide();
		if (IsWaiting("正在核查角色选择", "Verifying player selection"))
		{
			if (m_spy.IsInChatRoom())
				m_ticker.Update(ST_CHANNEL_STUFF);
		}
		else
		{
			m_auto.CleanDlg();
			ShowInfo("角色选择超时，准备重试", "Player selection timeout, retrying");
			
			if (m_bMuleMode)
				m_ticker.Update(ST_EXIT_CHARLIST, 3000);
			else
				m_ticker.Update(ST_CLOSE_D2);
		}

		break;

	case ST_CHANNEL_STUFF:
		CheckAndHide();
		ShowInfo("正在进入并检查Battle.net频道信息", "Verifying Battle.net channel stats");
		m_dwJoinFail = 0;
		if (!m_data.sChannel.IsEmpty())
		{
			ShowInfo("进入频道\"%s\"", "Joining channel \"%s\"", m_data.sChannel);
			m_auto.JoinChannel(m_data.sChannel, NULL);
			m_ticker.Update(ST_PREPARE_CREATE, 8000);			
		}
		else
		{
			m_ticker.Update(ST_PREPARE_CREATE);
		}

		break;

	case ST_PREPARE_CREATE:
		CheckAndHide();
		if (!IsWaiting("准备创建新游戏", "Prepare to create game"))
		{
			::srand(::GetTickCount());
			if (m_bMuleMode)
				m_nNextGameCrTime = 0;
			m_nNextGameCrTime = max(0, m_nNextGameCrTime);
			int nDiff = abs(m_pGlobal->nRand2 - m_pGlobal->nRand1);
			if (nDiff)
				m_nNextGameCrTime += abs(m_pGlobal->nRand1) + (::rand() % nDiff);
			else
				m_nNextGameCrTime += abs(m_pGlobal->nRand1);
			m_ticker.Update(ST_WAIT_BEFORE_CREATE, max(1000, m_nNextGameCrTime * 1000));
		}
		
		break;

	case ST_WAIT_BEFORE_CREATE:
		CheckAndHide();
		if (!IsWaiting("准备创建新游戏", "Prepare to create game"))
			m_ticker.Update(ST_CREATE_GAME);
		break;

	case ST_CREATE_GAME:
		CheckAndHide();
		CreateGame();
		break;

	case ST_CHANNEL_RETRY:	
		CheckAndHide();
		if (IsWaiting("正在探测角色状况", "Detecting player status"))
		{
			if (m_spy.IsInChatRoom())
				m_ticker.Update(ST_PREPARE_CREATE);
		}
		else
		{
			if (m_spy.IsBNetLoginSuccessful())
			{
				if (m_spy.GetCharCount() > 0)
				{
					// we are in char sel screen now
					m_ticker.Update(ST_SELECT_CHAR);
				}
				else
				{
					// we are in login screen now
					m_ticker.Update(ST_LOGIN);
				}
			}
			else
			{
				// we are completely dropped from bnet
				m_ticker.Update(ST_RECONNECT);
			}
		}
		
		break;

	case ST_BEFORE_CHECKGAME:
		CheckAndHide();
		if (!IsWaiting("准备检查角色状况", "Prepare to check player status"))
			m_ticker.Update(ST_CHECK_GAME, m_pGlobal->nCreateDelay * 1000);
		break;

	case ST_CHECK_GAME:
		CheckAndHide();
		CheckGame();
		break;

	case ST_IN_GAME:	
		CheckAndHide();
		// in game now
		WhileInGame();
		break;

	case ST_CHECK_EXIT_GAME:
		CheckAndHide();
		if (m_bMuleMode)
		{
			ShowInfo("游戏退出，准备访问下一个角色", "Accessing next player");
			m_ticker.Update(ST_EXIT_CHANNEL, (rand() % 6000) + 4000);
		}
		else
		{
			ShowInfo("游戏退出，准备重建", "Game quit, prepare to recreate");
			m_ticker.Update(ST_PREPARE_CREATE);	
		}

		break;

	case ST_EXIT_CHANNEL:
		CheckAndHide();
		if (!IsWaiting("正在退出Battle.net频道", "Quitting Battle.net channel"))
		{
			m_auto.SendEscape();
			m_ticker.Update(ST_EXIT_CHARLIST, 8000);
		}		
		
		break;

	case ST_EXIT_CHARLIST:
		CheckAndHide();
		if (m_spy.GetCharCount() > 0 || m_ticker.IsTickEnded())
		{
			//m_auto.SendEscape(); // close the "Player not found" window

			int nType = m_mule.GetNextLogin(m_data.login);
			if (nType == MULE_SAME_ACCOUNT)
			{
				// log on next char
				m_ticker.Update(ST_SHOW_CHARLIST);
			}
			else if (nType == MULE_DIFF_ACCOUNT)
			{
				// no more char in this mule account, log on next account
				m_auto.SendEscape();
				ShowInfo("正在退出角色选择界面", "Exiting player selection");				
				m_ticker.Update(ST_RECONNECT, 8000);
			}
			else if (nType == MULE_DIFF_REALM)
			{
				m_ticker.Update(ST_CLOSE_D2);
			}
			else
			{
				Stop(TRUE);
			}
		}

		break;
		
	case ST_RECONNECT:
		CheckAndHide();
		if (m_ticker.IsTickEnded())
		{
			m_dwConnectRetry = 0;
			m_ticker.Update(m_spy.IsInChatRoom() ? ST_PREPARE_CREATE : ST_CONNECTBNET);
		}
		
		break;

	default:
		break;
	}
}

const CSIGThreadData* CSIGInstance::GetData() const
{
	return &m_data;
}

const CMule* CSIGInstance::GetMule() const
{
	return m_bMuleMode ? &m_mule : NULL;
}

BOOL CSIGInstance::SetData(const CSIGThreadData* pData)
{
	if (pData == NULL)
		return FALSE;

	m_data = *pData;
	m_dictionary.SetRule(&pData->nameRule);
	UpdateLogPath();
	return TRUE;
}

BOOL CSIGInstance::SetMule(const CMule* pMule)
{
	if (!m_bMuleMode || pMule == NULL)
		return FALSE;

	m_mule.Copy(pMule);
	return TRUE;
}

BOOL CSIGInstance::CheckAndCloseErrors()
{
	int nErr = 0;

	HWND hWnd = NULL;
		
	for (int i = 0; ERROR_MSG[i] != NULL; i++)
	{
		hWnd = ::FindWindow(NULL, ERROR_MSG[i]);
		if (hWnd != NULL)
		{
			DWORD dwID = 0;
			GetWindowThreadProcessId(hWnd, &dwID);
			if (dwID > 0 && dwID == m_dwD2ProcessID)
			{
				nErr++;
				DWORD dwDummy;
				::SendMessageTimeout(hWnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG, 2000, &dwDummy);
			}			
		}
	}

	return nErr > 0;
}

BOOL CSIGInstance::VerifyGame()
{
	if (m_hD2Wnd == NULL)
		return TRUE;

	if (CheckAndCloseErrors())
		return FALSE;

	return ::IsWindow(m_hD2Wnd);
}

HWND CSIGInstance::GetD2HWnd() const
{
	return m_hD2Wnd;
}

LPCSTR CSIGInstance::GetName() const
{
	return m_data.sName;
}

DWORD CSIGInstance::GetRunningLength() const
{
	return IsStarted() ? (::time(NULL) - m_stats.lStart) / 1000 : 0;
}

int CSIGInstance::GetGameCount() const
{
	return m_nGames;
}

BOOL CSIGInstance::LeaveGame() const
{
	if (m_hD2Wnd == NULL || m_dwD2ProcessID == 0 || !m_bGameJoined)
		return FALSE;

	HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, 0, m_dwD2ProcessID);
	if (hProc == NULL)
		return FALSE;

	DWORD dw0 = 0;
	DWORD dwDummy;
	DWORD oldprot;
	VirtualProtect(LPVOID(0x6FAA0000+0x000D4538), 4, PAGE_READWRITE, &oldprot);	
	BOOL bOK = WriteProcessMemory(hProc, LPVOID(0x6FAA0000+0x000D4538), &dw0, 4, &dwDummy);
	VirtualProtect(LPVOID(0x6FAA0000+0x000D4538), 4, oldprot, &dwDummy);
	::CloseHandle(hProc);

	if (!bOK)
		return FALSE;

	::SendMessage(m_hD2Wnd, WM_CLOSE, 0, 0);
	return TRUE;
}

int CSIGInstance::GetPlayerHPPercent()
{
	WORD dwHP = (WORD)m_playerStats.GetStats(D2PS_HP);
	WORD dwMaxHP = (WORD)m_playerStats.GetStats(D2PS_MAXHP);
	if (dwMaxHP == 0)
		return 100;

	return (int)((float)dwHP / (float)dwMaxHP * 100.0);
}

LPCPLAYER CSIGInstance::GetPlayer() const
{
	return m_bGameJoined ? &m_player : NULL;
}

BOOL CSIGInstance::HideD2Wnd() const
{
	if (m_hD2Wnd == NULL)
		return FALSE;

	::ShowWindow(m_hD2Wnd, SW_HIDE);
	return TRUE;
}

BOOL CSIGInstance::Verify() const
{
	if (m_data.sD2Executable.IsEmpty() || m_data.sName.IsEmpty())
		return FALSE;

	return m_bMuleMode ? m_mule.Verify() : m_data.login.Verify();
}

void CSIGInstance::WhileInGame()
{
	m_stats.nCurGameLen++;
	m_stats.nTotalInGame++;

	if (m_bFirstGame)
		m_stats.nMinGame = m_stats.nCurGameLen;
	if (m_bFirstGame || m_stats.nCurGameLen > m_stats.nMaxGame)
		m_stats.nMaxGame = m_stats.nCurGameLen;

	int nLimit = GetInGameLimit();
	if (nLimit > 0)
	{
		int nRemain = nLimit - m_stats.nCurGameLen;
		if (nRemain < 0)
		{
			ShowInfo("最大游戏时限已到, 强制退出", "Maximum in-game length exceeded, force leaving game");
			if (!LeaveGame())
				m_ticker.Update(ST_CLOSE_D2);
		}
		else
		{
			THREADMSG msg = { 0 };
			msg.lpszMsg = NULL;
			msg.nRemaining = nRemain;
			::SendMessage(g_hViewWnd, WM_INST_STRING, (WPARAM)this, (LPARAM)&msg);
		}
	}
}

int CSIGInstance::GetInGameLimit() const
{
	return m_bMuleMode ? m_mule.GetGameLen() : m_data.nInGameLimit;
}

void CSIGInstance::UpdateLogPath()
{
	if (m_data.sName.GetLength())
	{
		CString sPath;
		sPath.Format("%s\\Log\\%s.txt", AfxGetAppEx()->GetAppDir(), GetName());
		m_logger.SetFilePath(sPath);
	}
}

void CSIGInstance::ShowThreadLog()
{
	m_logger.Flush();
	::ShellExecute(NULL, "open", "notepad.exe", m_logger.GetFilePath(), NULL, SW_SHOW);
}

LPCSIGSTATS CSIGInstance::GetStats() const
{
	return &m_stats;
}

BOOL CSIGInstance::IsInGame() const
{
	return m_bGameJoined;
}

void CSIGInstance::CheckAntiIdleDLL() const
{
	//if (m_pGlobal)
}

LPCSTR CSIGInstance::GetCustLog(int nIdx) const
{
	return m_data.aCustomLogs.IsIndexValid(nIdx) ? (LPCSTR)m_data.aCustomLogs[nIdx] : NULL;
}

int CSIGInstance::GetCurLogIdx() const
{
	return m_data.nCurLog;
}

void CSIGInstance::SetCurLogIdx(int nIdx)
{
	m_data.nCurLog = nIdx;
}

void CSIGInstance::AddCustomLog(LPCSTR lpszPath)
{
	m_data.AddCustomLog(lpszPath);
}

void CSIGInstance::OnCDKeyBanned()
{
	m_nBannedTest = 0;
	ShowInfo("*** CDKey Banned! ***", "*** CDKey Banned! ***");
	if (m_data.nBanRule == BAN_RETRY)
	{
		KillD2Process();
		m_ticker.Update(ST_BAN_WAIT, max(5000, m_data.nBanRetry * 1000));
		return;
	}

	Stop(TRUE);
	if (m_data.nBanRule == BAN_SWITCH && !m_data.sBanSwitchThread.IsEmpty())
	{
		CSIGData* psd = GlobalGetSIGData();
		if (psd == NULL)
			return;

		CSIGInstance* pInstance = psd->FindThread(m_data.sBanSwitchThread);
		if (pInstance)
			pInstance->Start();
	}	
}

BOOL CSIGInstance::IsWaiting(LPCSTR lpszChs, LPCSTR lpszEng)
{
	if (m_ticker.IsTickEnded())
		return FALSE;

	LPCSTR lpsz = g_bLanguage ? lpszChs : lpszEng;
	if (lpsz == NULL || lpsz[0] == 0)
		return TRUE;

	m_logger.Log(lpsz);
	THREADMSG msg = { 0 };
	msg.lpszMsg = lpsz;
	msg.nRemaining = m_ticker.GetRemainer() / 1000;
	::SendMessage(g_hViewWnd, WM_INST_STRING, (WPARAM)this, (LPARAM)&msg);
	return TRUE;
}

void CSIGInstance::OnRealmDown()
{
	// retry every 10 minutes
	ShowInfo("可能遇到Realm Down，10分钟后重试", "Possibly Realm Down, retry in 10 minutes");
	KillD2Process();
	m_ticker.Update(ST_BAN_WAIT, 600 * 1000);	
}

void CSIGInstance::CheckAndHide()
{
	if (IsStarted() && !AfxGetMainWnd()->IsWindowVisible())
		HideD2Wnd();
}

BOOL CSIGInstance::UpdateAutoloadScripts(LPCSTR lpszD2j) const
{
	if (lpszD2j == NULL || lpszD2j[0] == 0)
		return FALSE;

	CStdioFile file;
	BOOL bFileOpened = FALSE;

	try 
	{
		bFileOpened = file.Open(lpszD2j, CFile::modeWrite | CFile::modeCreate);
	} catch (...)
	{
		if (bFileOpened)
			file.Close();

		return FALSE;
	}

	if (!bFileOpened)
		return FALSE;

	COleDateTime now = COleDateTime::GetCurrentTime();
	CString sTimeTag;
	sTimeTag.Format(_T("// Abin, %s\n"), now.Format());
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n"));
	file.WriteString(_T("//         Default.d2j (D2JSP Entry Point)\n//\n"));
	file.WriteString(_T("// Contents of this file were automatically generated by SIG.\n"));
	file.WriteString(_T("// Please do not modify unless you want to manually configure your\n"));
	file.WriteString(_T("// scripts and load them by yourself.\n//\n"));
	file.WriteString(sTimeTag);
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n\n"));
	file.WriteString(_T("function main()\n{\n"));

	// insert auto load scripts here
	CStringArray aScripts;
	SplitString(m_data.sD2JSPScripts, ",", aScripts);
	for (int i = 0; i < aScripts.GetSize(); i++)
	{
		CString sLine;
		sLine.Format("\tload(\"bots/%s.d2j\");\n", aScripts[i]);
		file.WriteString(sLine);
	}

	file.WriteString(_T("}\n\n"));
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n"));
	file.WriteString(_T("//         End of Default.d2j\n"));
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n\n"));
	file.Close();
	return TRUE;
}
