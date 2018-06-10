#ifndef __AUTOITFUNCS_H__
#define __AUTOITFUNCS_H__

#include <windows.h>
#include "External\\MsgSender.h"

class CAutoFunc
{
public:
	
	CAutoFunc();
	~CAutoFunc();
	
	void SetTargetHWnd(HWND hWnd);
	static POINT GetD2WndCursorPos(HWND hD2Wnd);

	// Out-game functions
	void Login(LPCTSTR lpAccount, LPCTSTR lpPassword);
	void SelectChar(int nCharPosition);
	void JoinChannel(LPCTSTR lpChannelName, LPCTSTR lpAwayMsg);
	void CreateGame(LPCTSTR lpName, LPCTSTR lpPwd, int nDifficulty = 0, DWORD dwOpenDelay = 2000, DWORD dwSwitchDelay = 200);
	void CancelQueue();
	void JoinGame(LPCTSTR lpName, LPCTSTR lpPwd, DWORD dwOpenDelay = 2000, DWORD dwSwitchDelay = 200);
	BOOL SendSysKey(UINT nKeyCode);
	void CleanDlg();
	void CancelConnect();
	void SendEscape();
	void SetKeyDelay(DWORD dwDelay);
	void ConnectToBNet();

private:

	BOOL StartThreadProc(LPTHREAD_START_ROUTINE lpfn);
	void StopThreadProc();
	
	// static functions for multi-threading calls
	static DWORD WINAPI __CreateGame(LPVOID lpParam);
	static DWORD WINAPI __JoinGame(LPVOID lpParam);
	static DWORD WINAPI __Login(LPVOID lpParam);
	static DWORD WINAPI __ConnectToBNet(LPVOID lpParam);
	static DWORD WINAPI __JoinChannel(LPVOID lpParam);

	CMsgSender m_sender;
	HWND m_hD2Wnd;
	BOOL m_bTrayed;
	char m_szString1[256];
	char m_szString2[256];
	DWORD m_dwOpenDelay;
	DWORD m_dwSwitchDelay;
	int m_nDifficulty;
	HANDLE m_hThread;
	volatile BYTE m_iStop;
};

#endif