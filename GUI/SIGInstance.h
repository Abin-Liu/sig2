////////////////////////////////////////////////////////////////
// SIGInstance.h
//
// SIG control running instance.
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////

#ifndef __SIGINSTANCE_H__
#define __SIGINSTANCE_H__

#include "SIGDataDef.h"
#include "External\\StateTicker.h"
#include "External\\ArrayEx.h"
#include "External\\LinkedList.h"
#include "D2Spy.h"
#include "AutoFuncs.h"
#include "NameDictionary.h"
#include "Mule.h"
#include "External\\RealmInfo.h"
#include "External\\StreamBuffer.h"
#include "D2PlayerStats.h"
#include "External\\Logger.h"

#define INSTANCE_TICK	1000
#define WND_TICK		500

extern LPCVOID g_pCurrentLaunchingInst;

class CSIGInstance
{
public:		
	void AddCustomLog(LPCSTR lpszPath);
	void SetCurLogIdx(int nIdx);
	int GetCurLogIdx() const;
	LPCSTR GetCustLog(int nIdx) const;

	CSIGInstance(LPCGLOBALDATA pGlobal, const CStringArrayEx* pDictionary, BOOL bMuleMode = FALSE);
	virtual ~CSIGInstance();

	BOOL LoadData(CStreamBuffer& sb, LPCSTR lpszPassword);
	BOOL SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const;
	const CSIGThreadData* GetData() const;
	const CMule* GetMule() const;
	BOOL SetData(const CSIGThreadData* pData);
	BOOL SetMule(const CMule* pMule);
	LPCSTR GetName() const;
	HWND GetD2HWnd() const;
	int GetGameCount() const;
	DWORD GetRunningLength() const;
	BOOL LeaveGame() const;
	int GetPlayerHPPercent();
	LPCPLAYER GetPlayer() const;
	BOOL Verify() const;
	BOOL HideD2Wnd() const;
	void ShowThreadLog();
	LPCSIGSTATS GetStats() const;
	BOOL IsInGame() const;

	void OnInstanceTick();
	void OnWndTick();

	BOOL Start(int nTimeLimit = -1, int nGameLimit = -1);
	BOOL Pause();
	BOOL Resume();
	BOOL Stop(BOOL bKillD2);
	BOOL IsStarted() const;
	BOOL IsPaused() const;
	BOOL ShowD2Wnd(BOOL bBringToTop);
	void KillD2Process();

private:

	BOOL UpdateAutoloadScripts(LPCSTR lpszD2j) const;
	void CheckAndHide();
	void OnRealmDown();
	BOOL IsWaiting(LPCSTR lpszChs = NULL, LPCSTR lpszEng = NULL);
	void OnCDKeyBanned();
	void CheckAntiIdleDLL() const;
	void UpdateLogPath();
	int GetInGameLimit() const;
	void WhileInGame();

	BOOL CheckAndCloseErrors();
	BOOL VerifyGame();
	void CheckGame();
	void CreateGame();
	void CheckCharList();
	void OnGameLeave();
	void OnGameJoin();
	void MoveD2WndIn();
	void MoveD2WndOut();
	BOOL LoadPlugins();
	BOOL VerifyD2Wnd();
	BOOL LaunchDiablo();
	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);	
	void CheckAndKillD2s();	
	static BOOL IsD2Wnd(HWND hWnd);
	static void KillD2Process(HWND hD2Wnd, DWORD dwProcessID = 0);	
	void ShowInfo(LPCSTR lpszChsFmt, LPCSTR lpszEngFmt, ...);	
	void PostError(LPCSTR lpszChsFmt, LPCSTR lpszEngFmt, ...);

	BOOL m_bMuleMode;
	CMule m_mule;
	CSIGThreadData m_data;
	CNameDictionary m_dictionary;
	CStateTicker m_ticker;
	LPCGLOBALDATA const m_pGlobal;
	HWND m_hD2Wnd;
	DWORD m_dwD2ProcessID;
	WINDOWPLACEMENT m_wp;
	CArrayEx<HWND, HWND> m_aD2Snapshot;
	CD2Spy m_spy;
	CAutoFunc m_auto;
	DWORD m_dwConnectRetry;
	BOOL m_bForceD2OnTop;
	BOOL m_bPlacementChanged;
	BOOL m_bGameJoined;
	DWORD m_dwJoinFail;
	DWORD m_dwLoginFail;
	CString m_sInfoBeforePaused;
	int m_nGames;
	int m_nTimeLimit;
	int m_nGameLimit;
	CD2PlayerStats m_playerStats;
	PLAYER m_player;
	DWORD m_dwHP;
	DWORD m_dwMaxHP;
	int m_nNextGameCrTime;
	CLogger m_logger;
	SIGSTATS m_stats;
	int m_nBannedTest;
	int m_nRealmDownTest;
	BOOL m_bFirstGame;
};

void CALLBACK FreeInstance(CSIGInstance*& pData);
typedef CLinkedList<CSIGInstance*, CSIGInstance*, FreeInstance> CThreadList;

#endif // __SIGINSTANCE_H__