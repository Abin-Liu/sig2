//////////////////////////////////////////////////////////////////////
// SIGDataDef.cpp
//
// SIG control data definition.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SIGDataDef.h"
#include "External\\Encrypt.h"

//default delay
#define DEF_DELAY_LAUNCH	30
#define DEF_DELAY_LOGIN		15
#define DEF_DELAY_CHARSEL	10
#define DEF_DELAY_JOIN		120
#define DEF_DELAY_ERROR		15
#define DEF_DELAY_MINCONN	5

void SetDefaultGlobalData(LPGLOBALDATA pData)
{
	ASSERT(pData);
	::memset(pData, 0, sizeof(GLOBALDATA));
	pData->nMaxD2Process = 8;
	pData->nLaunchDelay = DEF_DELAY_LAUNCH;
	pData->nConnectDelay = DEF_DELAY_LOGIN;
	pData->nLoginDelay = DEF_DELAY_LOGIN;
	pData->nCharSelDelay = DEF_DELAY_CHARSEL;
	pData->nCreateDelay = DEF_DELAY_JOIN;
	pData->nRestartDelay = DEF_DELAY_ERROR;
	pData->nRand1 = 3;
	pData->nRand2 = 10;
	pData->nButtonDelay = 3;
}

CLoginData::CLoginData()
{
	nCharIdx = 0;
}

const CLoginData& CLoginData::operator=(const CLoginData& rhs)
{
	nCharIdx = rhs.nCharIdx;
	sAccount = rhs.sAccount;
	sBnetIP = rhs.sBnetIP;
	sPassword = rhs.sPassword;
	sSubRealm = rhs.sSubRealm;
	return *this;
}

void CLoginData::SaveData(CStreamBuffer &sb, LPCSTR lpszPassword) const
{
	char szEnc[256] = "";
	Encrypt(sPassword, szEnc, 255, lpszPassword);	
	sb << nCharIdx << sAccount << sBnetIP << sSubRealm << szEnc;
}

BOOL CLoginData::LoadData(CStreamBuffer &sb, LPCSTR lpszPassword)
{
	sb >> nCharIdx >> sAccount >> sBnetIP >> sSubRealm >> sPassword;	
	char szDec[256] = "";
	Decrypt(sPassword, szDec, 255, lpszPassword);
	sPassword = szDec;
	return TRUE;
}

CSIGThreadData::CSIGThreadData()
{
	nDifficulty = 0;
	nInGameLimit = 0;
	nHackType = 0;
	bAllowModifyScript = TRUE;
	bAntiIdle = FALSE;
	nMin2Games = 0;
	nBanRule = BAN_STOP;
	nBanRetry = 3600;
	::memset(&nameRule, 0, sizeof(nameRule));
}

BOOL CSIGThreadData::LoadData(CStreamBuffer& sb, LPCSTR lpszPassword)
{
	CString aStr[4];

	sb.StreamRead(&nameRule, sizeof(nameRule));
	sb >> bAllowModifyScript >> nDifficulty >> nHackType >> nInGameLimit >> sChannel
		>> sD2Executable >> sD2HackitDll >> sD2JspDll >> sD2JSPScripts >> sModules 
		>> sName >> sParamLine >> bAntiIdle >> nMin2Games >> nBanRule >> nBanRetry
		>> sBanSwitchThread >> aStr[0] >> aStr[1] >> aStr[2] >> aStr[3] >> nCurLog;

	login.LoadData(sb, lpszPassword);
	aCustomLogs.RemoveAll();
	
	for (int i = 0; i < 4; i++)
	{
		if (!aStr[i].IsEmpty())
			aCustomLogs.Add(aStr[i]);
	}

	return TRUE;
}

void CSIGThreadData::SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const
{
	CString aStr[4];
	for (int i = 0; i < aCustomLogs.GetSize() && i < 4; i++)
		aStr[i] = aCustomLogs[i];
	
	sb.Append(&nameRule, sizeof(nameRule));
	sb << bAllowModifyScript << nDifficulty << nHackType << nInGameLimit << sChannel
		<< sD2Executable << sD2HackitDll << sD2JspDll << sD2JSPScripts << sModules 
		<< sName << sParamLine << bAntiIdle << nMin2Games << nBanRule << nBanRetry
		<< sBanSwitchThread << aStr[0] << aStr[1] << aStr[2] << aStr[3] << nCurLog;
	login.SaveData(sb, lpszPassword);
}

const CSIGThreadData& CSIGThreadData::operator=(const CSIGThreadData& rhs)
{
	bAllowModifyScript = rhs.bAllowModifyScript;
	nDifficulty = rhs.nDifficulty;
	nHackType = rhs.nHackType;
	nInGameLimit = rhs.nInGameLimit;
	sChannel = rhs.sChannel;
	sD2Executable = rhs.sD2Executable;
	sD2HackitDll = rhs.sD2HackitDll;
	sD2JspDll = rhs.sD2JspDll;
	sD2JSPScripts = rhs.sD2JSPScripts;
	sModules = rhs.sModules;
	sName = rhs.sName;
	sParamLine = rhs.sParamLine;
	login = rhs.login;
	nameRule = rhs.nameRule;
	aCustomLogs.Copy(rhs.aCustomLogs);
	nCurLog = rhs.nCurLog;
	bAntiIdle = rhs.bAntiIdle;
	nMin2Games = rhs.nMin2Games;
	nBanRule = rhs.nBanRule;
	nBanRetry = rhs.nBanRetry;
	sBanSwitchThread = rhs.sBanSwitchThread;
	return *this;
}

void CSIGThreadData::AddCustomLog(LPCSTR lpszPath)
{
	if (lpszPath == NULL || lpszPath[0] == 0)
		return;

	for (int i = 0; i < aCustomLogs.GetSize(); i++)
	{
		if (aCustomLogs[i].CompareNoCase(lpszPath) == 0)
			return;
	}

	if (aCustomLogs.GetSize() >= 4)
		aCustomLogs.RemoveAt(0);

	if (lpszPath && lpszPath[0])
		nCurLog = aCustomLogs.Add(lpszPath);
}

CScheduleData::CScheduleData()
{
	nStartRule = 0;
	nStartParam = 0;
	nEndRule = 0;	
	nEndParam = 0;	
}

BOOL CScheduleData::LoadData(CStreamBuffer& sb)
{
	sb >> nStartRule >> nStartParam >> nEndRule >> nEndParam >> sThreadName;
	return TRUE;
}

void CScheduleData::SaveData(CStreamBuffer& sb) const
{
	sb << nStartRule << nStartParam << nEndRule << nEndParam << sThreadName;
}

const CScheduleData& CScheduleData::operator=(const CScheduleData& rhs)
{
	nStartRule = rhs.nStartRule;
	nStartParam = rhs.nStartParam;
	nEndRule = rhs.nEndRule;
	nEndParam = rhs.nEndParam;
	sThreadName = rhs.sThreadName;
	return *this;
}

CMuleData::CMuleData()
{
	nCount = 0;
	nCurIdx = 0;
}

BOOL CMuleData::LoadData(CStreamBuffer& sb, LPCSTR lpszPassword)
{
	nCurIdx = 0;
	sb >> nCount;
	return login.LoadData(sb, lpszPassword);
}

void CMuleData::SaveData(CStreamBuffer& sb, LPCSTR lpszPassword) const
{
	sb << nCount;
	login.SaveData(sb, lpszPassword);
}

const CMuleData& CMuleData::operator=(const CMuleData& rhs)
{
	login = rhs.login;
	nCount = rhs.nCount;
	nCurIdx = rhs.nCurIdx;
	return *this;
}

BOOL CMuleData::CheckLogin(CLoginData& rLogin)
{
	if (nCount < 1 || nCurIdx >= nCount)
		return FALSE;

	rLogin = login;
	rLogin.nCharIdx = nCurIdx;
	nCurIdx++;
	return TRUE;
}

BOOL CLoginData::Verify() const
{
	return sAccount.GetLength() && sPassword.GetLength();
}

BOOL CMuleData::Verify() const
{
	return nCount > 0 && login.Verify();
}
