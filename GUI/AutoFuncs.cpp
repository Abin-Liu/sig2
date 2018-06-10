#include "stdafx.h"
#include <stdio.h>
#include "AutoFuncs.h"
#include "D2WndPosition.h"

#define AUTO_SLEEP(x)	for (i = 0; i < x; i += 100)\
						{\
							if (pAuto->m_iStop)\
								return 0;\
							::Sleep(100);\
						}

CAutoFunc::CAutoFunc()
{
	m_hD2Wnd = NULL;
	m_bTrayed = TRUE;
	m_iStop = 0x00;
	m_szString1[0] = 0;
	m_szString1[0] = 0;
	m_dwOpenDelay = 2000;
	m_dwSwitchDelay = 2000;
	m_nDifficulty = 0;
	m_hThread = NULL;
}

CAutoFunc::~CAutoFunc()
{
	StopThreadProc();
}

POINT CAutoFunc::GetD2WndCursorPos(HWND hD2Wnd)
{
	CRect rcWnd, rcClient;
	::GetWindowRect(hD2Wnd, &rcWnd);
	::GetClientRect(hD2Wnd, &rcClient);
	const int cx = (rcWnd.Width() - rcClient.Width()) / 2; // border frame width
	const int cy = rcWnd.Height() - rcClient.Height() - cx; // title bar height
	POINT pt;
	::GetCursorPos(&pt);
	pt.x -= rcWnd.left + cx;
	pt.y -= rcWnd.top + cy;
	return pt;
}

DWORD CAutoFunc::__CreateGame(LPVOID lpParam)
{
	DWORD i = 0;
	const CAutoFunc* pAuto = (const CAutoFunc*)lpParam;
	if (pAuto == NULL)
		return 0;

	POINT pt;

	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	// location of the "Create" button
	pAuto->m_sender.LeftClick(CREATE_BUTTON.x, CREATE_BUTTON.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);

	AUTO_SLEEP(pAuto->m_dwOpenDelay);

	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LeftClick(CREATE_BUTTON.x, CREATE_BUTTON.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	AUTO_SLEEP(500);

	// location of "game name" text box
	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LDblClick(CREATENAME_BOX.x, CREATENAME_BOX.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	AUTO_SLEEP(pAuto->m_dwSwitchDelay);
	
	pAuto->m_sender.SendString(pAuto->m_szString1); // send game name	
	AUTO_SLEEP(pAuto->m_dwSwitchDelay);

	if (_tcslen(pAuto->m_szString2) > 0)
	{
		pAuto->m_sender.SendSysKey(VK_TAB); // move to pwd editbox
		AUTO_SLEEP(pAuto->m_dwSwitchDelay);

		pAuto->m_sender.SendString(pAuto->m_szString2);	// send pwd
		AUTO_SLEEP(pAuto->m_dwSwitchDelay);
	}

	// select difficulties
	if (pAuto->m_nDifficulty > 0 && pAuto->m_nDifficulty <= 3)
	{
		pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
		pAuto->m_sender.LeftClick(DIFFICULTIES[pAuto->m_nDifficulty - 1].x, DIFFICULTIES[pAuto->m_nDifficulty - 1].y);
		pAuto->m_sender.MouseMove(pt.x, pt.y);
		AUTO_SLEEP(pAuto->m_dwSwitchDelay);
	}

	AUTO_SLEEP(1000);

	pAuto->m_sender.LeftClick(APPLY_CREATE.x, APPLY_CREATE.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y); // done

	return 0;
}

DWORD CAutoFunc::__Login(LPVOID lpParam)
{
	DWORD i = 0;
	const CAutoFunc* pAuto = (const CAutoFunc*)lpParam;
	if (pAuto == NULL)
		return 0;

	POINT pt;
	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);

	//if (!CRealmInfo::IsDefaultAccountSame(g_loginParam.szAccount))
	{
		pAuto->m_sender.LDblClick(ACCOUNTNAME_BOX.x, ACCOUNTNAME_BOX.y);
		pAuto->m_sender.MouseMove(pt.x, pt.y);
		AUTO_SLEEP(1000);

		pAuto->m_sender.LDblClick(ACCOUNTNAME_BOX.x, ACCOUNTNAME_BOX.y);
		pAuto->m_sender.MouseMove(pt.x, pt.y);
		AUTO_SLEEP(500);

		pAuto->m_sender.SendString(pAuto->m_szString1);
		AUTO_SLEEP(200);

		pAuto->m_sender.SendSysKey(VK_TAB);
		AUTO_SLEEP(500);
	}
	
	pAuto->m_sender.SendString(pAuto->m_szString2);
	AUTO_SLEEP(200);

	pAuto->m_sender.LeftClick(APPLY_LOGIN.x, APPLY_LOGIN.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y); // done

	return 0;
}

DWORD CAutoFunc::__ConnectToBNet(LPVOID lpParam)
{
	const CAutoFunc* pAuto = (const CAutoFunc*)lpParam;
	if (pAuto == NULL)
		return 0;

	POINT pt;
	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LeftClick(BNET_BUTTON.x, BNET_BUTTON.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	return 0;
}

DWORD CAutoFunc::__JoinChannel(LPVOID lpParam)
{
	DWORD i = 0;
	const CAutoFunc* pAuto = (const CAutoFunc*)lpParam;
	if (pAuto == NULL)
		return 0;

	POINT pt;

	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LeftClick(ENTER_CHAT.x, ENTER_CHAT.y);
	AUTO_SLEEP(5000);
	pAuto->m_sender.LDblClick(CHAT_INPUT.x, CHAT_INPUT.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	AUTO_SLEEP(500);	
	
	if (pAuto->m_szString2[0]!= _T('\0'))
	{
		char sz[256] = "";
		::sprintf(sz, "/away %s", pAuto->m_szString2);
		pAuto->m_sender.SendString(sz);
		AUTO_SLEEP(100);
		pAuto->m_sender.LeftClick(APPLY_CHAT.x, APPLY_CHAT.y);
		pAuto->m_sender.MouseMove(pt.x, pt.y); // done
		AUTO_SLEEP(500);
	}

	if (pAuto->m_szString1[0] != _T('\0'))
	{
		char sz[256] = "";
		::sprintf(sz, "/join %s", pAuto->m_szString1);
		pAuto->m_sender.SendString(sz);
		AUTO_SLEEP(500);
		pAuto->m_sender.LeftClick(APPLY_CHAT.x, APPLY_CHAT.y);
		pAuto->m_sender.MouseMove(pt.x, pt.y); // done
	}

	return 0;
}

DWORD CAutoFunc::__JoinGame(LPVOID lpParam)
{
	DWORD i = 0;
	const CAutoFunc* pAuto = (const CAutoFunc*)lpParam;
	if (pAuto == NULL)
		return 0;

	POINT pt;

	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	// location of the "Create" button
	pAuto->m_sender.LeftClick(JOIN_BUTTON.x, JOIN_BUTTON.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);

	AUTO_SLEEP(pAuto->m_dwOpenDelay);

	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LeftClick(JOIN_BUTTON.x, JOIN_BUTTON.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	AUTO_SLEEP(500);

	// location of "game name" text box
	pt = GetD2WndCursorPos(pAuto->m_hD2Wnd);
	pAuto->m_sender.LDblClick(JOINNAME_BOX.x, JOINNAME_BOX.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y);
	AUTO_SLEEP(pAuto->m_dwSwitchDelay);
	
	pAuto->m_sender.SendString(pAuto->m_szString1); // send game name	
	AUTO_SLEEP(pAuto->m_dwSwitchDelay);

	if (_tcslen(pAuto->m_szString2) > 0)
	{
		pAuto->m_sender.SendSysKey(VK_TAB); // move to pwd editbox
		AUTO_SLEEP(pAuto->m_dwSwitchDelay);

		pAuto->m_sender.SendString(pAuto->m_szString2);	// send pwd
		AUTO_SLEEP(pAuto->m_dwSwitchDelay);
	}	

	AUTO_SLEEP(500);

	pAuto->m_sender.LeftClick(APPLY_CREATE.x, APPLY_CREATE.y);
	pAuto->m_sender.MouseMove(pt.x, pt.y); // done

	return 0;
}

// Out-game functions
void CAutoFunc::Login(LPCTSTR lpAccount, LPCTSTR lpPassword)
{
	StopThreadProc();

	if (lpAccount == NULL || lpPassword == NULL)
		return;

	::memset(m_szString1, 0, sizeof(m_szString1));
	::memset(m_szString2, 0, sizeof(m_szString2));
	_tcsncpy(m_szString1, lpAccount, 255);
	_tcsncpy(m_szString2, lpPassword, 255);

	StartThreadProc(__Login);
}

void CAutoFunc::SelectChar(int nCharPosition)
{
	if (nCharPosition < 0 || nCharPosition > 7)
		nCharPosition = 0;

	POINT pt;
	pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LDblClick(CHAR_POSITIONS[nCharPosition].x,
		CHAR_POSITIONS[nCharPosition].y);

	m_sender.MouseMove(pt.x, pt.y);
}

void CAutoFunc::JoinChannel(LPCTSTR lpChannelName, LPCTSTR lpAwayMsg)
{
	StopThreadProc();

	if (lpChannelName == NULL || lpChannelName[0] == 0)
		return;

	::memset(m_szString1, 0, sizeof(m_szString1));
	::memset(m_szString2, 0, sizeof(m_szString2));
	_tcsncpy(m_szString1, lpChannelName, 255);

	if (lpAwayMsg)
		_tcsncpy(m_szString2, lpAwayMsg, 255);

	StartThreadProc(__JoinChannel);
}

void CAutoFunc::CreateGame(LPCTSTR lpName, LPCTSTR lpPwd, int nDifficulty, DWORD dwOpenDelay, DWORD dwSwitchDelay)
{
	StopThreadProc();

	if (lpName == NULL)
		return;

	::memset(m_szString1, 0, sizeof(m_szString1));
	::memset(m_szString2, 0, sizeof(m_szString2));
	_tcsncpy(m_szString1, lpName, 255);
	if (lpPwd)
		_tcsncpy(m_szString2, lpPwd, 255);

	m_nDifficulty = nDifficulty;
	m_dwOpenDelay = dwOpenDelay;
	m_dwSwitchDelay = dwSwitchDelay;
	StartThreadProc(__CreateGame);
}

void CAutoFunc::SetTargetHWnd(HWND hWnd)
{
	m_sender.SetTargetHWnd(hWnd);
	m_hD2Wnd = hWnd;
}

void CAutoFunc::ConnectToBNet()
{	
	StopThreadProc();
	StartThreadProc(__ConnectToBNet);
}

void CAutoFunc::SetKeyDelay(DWORD dwDelay)
{
	if (dwDelay >= 0 && dwDelay <= 500)
		m_sender.SetKeyDelay(dwDelay);
}

void CAutoFunc::SendEscape()
{
	m_sender.SendSysKey(VK_ESCAPE);
}

void CAutoFunc::CancelConnect()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CANCEL_CONNECT.x, CANCEL_CONNECT.y);
	m_sender.MouseMove(pt.x, pt.y);
}

void CAutoFunc::CleanDlg()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CLEAN_DLG.x, CLEAN_DLG.y);
	m_sender.MouseMove(pt.x, pt.y);
}

BOOL CAutoFunc::SendSysKey(UINT nKeyCode)
{
	return m_sender.SendSysKey(nKeyCode);
}

void CAutoFunc::JoinGame(LPCTSTR lpName, LPCTSTR lpPwd, DWORD dwOpenDelay, DWORD dwSwitchDelay)
{
	StopThreadProc();

	if (lpName == NULL)
		return;

	::memset(m_szString1, 0, sizeof(m_szString1));
	::memset(m_szString2, 0, sizeof(m_szString2));
	_tcsncpy(m_szString1, lpName, 255);
	if (lpPwd)
		_tcsncpy(m_szString2, lpPwd, 255);

	m_dwOpenDelay = dwOpenDelay;
	m_dwSwitchDelay = dwSwitchDelay;
	StartThreadProc(__JoinGame);
}

void CAutoFunc::CancelQueue()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CANCEL_QUEUE.x, CANCEL_QUEUE.y);
	m_sender.MouseMove(pt.x, pt.y);
}

BOOL CAutoFunc::StartThreadProc(LPTHREAD_START_ROUTINE lpfn)
{	
	StopThreadProc();

	if (lpfn == NULL)
		return FALSE;

	DWORD dwDummy;
	m_iStop = 0x00;
	m_hThread = ::CreateThread(NULL, 0, lpfn, (LPVOID)this, 0, &dwDummy);
	return m_hThread != NULL;
}

void CAutoFunc::StopThreadProc()
{
	m_iStop = 0x01;
	if (m_hThread)
	{
		if (::WaitForSingleObject(m_hThread, 2000) != WAIT_OBJECT_0)
			::TerminateThread(m_hThread, 0);
		m_hThread = NULL;
	}
}
