//////////////////////////////////////////////////////////////////////
// SIGDataDef.h
//
// SIG control data definition.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __SIGDATADEF_H__
#define __SIGDATADEF_H__

#include <windows.h>
#include "External\\StreamBuffer.h"
#include "External\\ArrayEx.h"

enum { HACK_NONE = 0, HACK_D2HACKIT, HACK_D2JSP };
enum { DIFF_NA = 0, DIF_NORMAL, DIF_NM, DIF_HELL };
enum { SCH_START_NOW = 0, SCH_START_AFTERPREVSTART, SCH_START_AFTERPREVSTOP };
enum { SCH_END_NEVER = 0, SCH_END_LENGTH, SCH_END_GAMES };
enum { NAME_RANDOM = 0, NAME_PREFIX, NAME_DICRANDOM, NAME_DICTURN };
enum { BAN_STOP = 0, BAN_RETRY, BAN_SWITCH };

// Global setting
typedef struct tagGlobalData
{
	int nMaxD2Process;
	int nLaunchDelay;
	int nConnectDelay;
	int nLoginDelay;
	int nCharSelDelay;
	int nCreateDelay;
	int nRestartDelay;
	int nRand1;
	int nRand2;
	int nButtonDelay;
} GLOBALDATA, *LPGLOBALDATA;
typedef const tagGlobalData* LPCGLOBALDATA;

typedef struct tagNameRule
{
	int nRule;
	BOOL bUseFixedPwd;
	char szNamePrefix[11];
	char szFixedPwd[16];
} NAMERULE, *LPNAMERULE;
typedef const tagNameRule* LPCNAMERULE;

typedef struct tagSIGStats
{
	time_t lStart;
	time_t lStop;
	int nTotalRunning;
	int nTotalInGame;
	int nMinGame;
	int nMaxGame;
	int nLogins;
	int nLoginOKs;
	int nGames;
	int nGameOKs;
	int nCrashes;
	int nCurGameLen;
} SIGSTATS, *LPSIGSTATS;
typedef const tagSIGStats* LPCSIGSTATS;

typedef struct tagThreadMsg
{
	LPCSTR lpszMsg;
	int nRemaining;
} THREADMSG, *LPTHREADMSG;
typedef const tagThreadMsg* LPCTHREADMSG;

class CScheduleData
{
public:

	CScheduleData();
	BOOL LoadData(CStreamBuffer& sb);
	void SaveData(CStreamBuffer& sb) const;
	const CScheduleData& operator=(const CScheduleData& rhs);

	CString sThreadName;
	int nStartRule;
	int nStartParam;
	int nEndRule;	
	int nEndParam;
};

class CLoginData
{
public:
	BOOL Verify() const;

	CLoginData();

	BOOL LoadData(CStreamBuffer& sb, LPCSTR lpszPassword);
	void SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const;

	const CLoginData& operator=(const CLoginData& rhs);
	
	CString sBnetIP;
	CString sSubRealm;
	CString sAccount;
	CString sPassword;
	int nCharIdx;
};

class CSIGThreadData
{
public:

	CSIGThreadData();
	BOOL LoadData(CStreamBuffer& sb, LPCSTR lpszPassword);
	void SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const;
	const CSIGThreadData& operator=(const CSIGThreadData& rhs);
	void AddCustomLog(LPCSTR lpszPath);

	int nDifficulty;
	int nInGameLimit;
	int nHackType;
	BOOL bAllowModifyScript;
	BOOL bAntiIdle;
	int nMin2Games;
	int nBanRule;
	int nBanRetry;
	CString sBanSwitchThread;

	CString sName;
	CString sD2Executable;
	CString sParamLine;
	CString sChannel;	
	CString sD2HackitDll;
	CString sD2JspDll;
	CString sModules;	
	CString sD2JSPScripts;
	int nCurLog;
	CArrayEx<CString, const CString&> aCustomLogs;
	NAMERULE nameRule;
	CLoginData login;	
};

class CMuleData
{
public:	

	CMuleData();
	BOOL LoadData(CStreamBuffer& sb, LPCSTR lpszPassword);
	void SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const;
	const CMuleData& operator=(const CMuleData& rhs);
	BOOL CheckLogin(CLoginData& rLogin);
	BOOL Verify() const;

	CLoginData login;
	int nCount;
	int nCurIdx;
};

void SetDefaultGlobalData(LPGLOBALDATA pData);

#endif // __SIGDATADEF_H__