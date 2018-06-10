#include <windows.h>
#include "TickThread.h"
#include "d2functions.h"
#include "d2functions.h"
#include "Constants.h"
#include <time.h>

typedef struct tagPlayerData
{
	WORD x;
	WORD y;
	WORD hp;
	WORD mana;
} PLAYERDATA, *LPPLAYERDATA;
typedef const tagPlayerData* LPCPLAYERDATA;

CTickThread g_ticker;
UnitPlayer* g_pPlayer = NULL;
PLAYERDATA g_data = { 0 };
DWORD g_dwLastActive = 0;

void CALLBACK TickProc(LPVOID lParam, LPCVBYTE lpStopFlag);
void OnGameJoin(UnitPlayer* pPlayer);
void AntiIdle();
void CheckPlayerData(UnitPlayer* pPlayer);
LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen);
void DisplayInfo(LPCSTR lpszInfo, BOOL bBottom);
int ReplaceColorSpecs(LPWSTR lpwsz);

void ClientStart()
{
	::srand((UINT)::time(NULL));
	g_ticker.Start(TickProc, NULL, 1000);
}

void ClientStop()
{
	g_ticker.Stop(3000);
	DisplayInfo("ÿc9Anti-Idle by Abin Removed", TRUE);
}

void OnGameJoin(UnitPlayer* pPlayer)
{
	DisplayInfo("ÿc9Anti-Idle by Abin Loaded", TRUE);
	g_pPlayer = pPlayer;
	::memset(&g_data, 0, sizeof(g_data));
	g_dwLastActive = ::GetTickCount();
}

void AntiIdle()
{
	static const char STR[] = "Anti-Idle";
	const int LEN = ::strlen(STR);
	BYTE aPacket[32] = { 0 };
	const int SEED = ::rand();

	if (SEED % 5)
	{		
		// send a random sound
		aPacket[0] = 0x3f;
		aPacket[1] = BYTE(SEED % 0x08) + 0x19;
		D2NET_Send(0, aPacket, 3);
	}
	else
	{
		aPacket[0] = 0x14;
		::memcpy(aPacket + 3, STR, LEN);
		D2NET_Send(0, aPacket, LEN + 6);		
	}
}

void CheckPlayerData(UnitPlayer* pPlayer)
{
	if (pPlayer == NULL)
		return;
	
	PLAYERDATA data = { 0 };
	data.x = pPlayer->wX;
	data.y = pPlayer->wY;
	data.hp = (WORD)D2COMMON_GetUnitModifiedStat(pPlayer, STAT_HP, 0);
	data.mana = (WORD)D2COMMON_GetUnitModifiedStat(pPlayer, STAT_MANA, 0);
	
	if (::memcmp(&data, &g_data, sizeof(data)))
	{
		::memcpy(&g_data, &data, sizeof(g_data));
		g_dwLastActive = ::GetTickCount();
	}
	else
	{
		// being idle
		if (::GetTickCount() - g_dwLastActive > 10000)
		{
			// anti-idle
			AntiIdle();
			g_dwLastActive = ::GetTickCount();
		}
	}
}

void CALLBACK TickProc(LPVOID lParam, LPCVBYTE lpStopFlag)
{
	UnitPlayer* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pPlayer == NULL)
		return;

	if (g_pPlayer == NULL)
		OnGameJoin(pPlayer);
	else
		CheckPlayerData(pPlayer);
}

LPWSTR ConvertToUnicode(LPCTSTR lpszAnsiString, LPWSTR lpUnicodeString, int maxlen)
{
	::MultiByteToWideChar(CP_ACP, 0, lpszAnsiString, -1, lpUnicodeString, maxlen);
	return lpUnicodeString;
}

void DisplayInfo(LPCSTR lpszInfo, BOOL bBottom)
{
	if (lpszInfo == NULL || lpszInfo[0] == 0 || D2CLIENT_PrintGameStringAtBottomLeft == NULL || D2CLIENT_PrintGameStringAtTopLeft == NULL || D2CLIENT_GetPlayerUnit() == NULL)
		return;

	wchar_t wsz[256] = { 0 };
	ConvertToUnicode(lpszInfo, wsz, 255);
	ReplaceColorSpecs(wsz);
	if (bBottom)
		D2CLIENT_PrintGameStringAtBottomLeft(wsz, 0);
	else
		D2CLIENT_PrintGameStringAtTopLeft(wsz, 0);
}

int ReplaceColorSpecs(LPWSTR lpwsz)
{
	// Prevent the "ÿc" symbol to be converted into wchar_t
	if (lpwsz == NULL)
		return 0;

	const int LEN = wcslen(lpwsz);
	int nCount = 0;
	for (int i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xf8f5) // Unicode 'ÿ'
		{
			lpwsz[i] = 0xff; // Ansi 'ÿ'
			nCount++;		
		}
	}

	return nCount;
}

