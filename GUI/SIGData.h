//////////////////////////////////////////////////////////////////////
// SIGData.h
//
// SIG control data configuration class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __SIGDATA_H__
#define __SIGDATA_H__

#include "External\\LinkedList.h"
#include "External\\TickThread.h"
#include "SIGInstance.h"
#include "SIGSchedule.h"

typedef void (CALLBACK *fnEnumInstProc)(const CSIGInstance* pInstance, LPARAM lParam);
typedef void (CALLBACK *fnEnumSchProc)(const CSIGSchedule* pSchedule, LPARAM lParam);

class CSIGData
{
public:	
		
	CSIGData();
	~CSIGData();

	int LoadData(LPCSTR lpszFile, LPCSTR lpszDicFile, LPCSTR lpszRealms);
	BOOL SaveData(LPCSTR lpszFile, LPCSTR lpszDicFile, LPCSTR lpszRealms) const;

	int GetThreadCount(int* pRunning = NULL) const;
	int GetScheduleCount(int* pRunning = NULL) const;

	const CSIGInstance* CreateInstance();
	BOOL EditInstance(CSIGInstance* pInstance);
	BOOL DeleteInstance(CSIGInstance* pInstance);

	const CSIGSchedule* CreateSchedule();
	BOOL EditSchedule(CSIGSchedule* pSchedule);
	BOOL DeleteSchedule(CSIGSchedule *pSchedule);

	CString GetUniqueName(BOOL bThreadOrSchedule, LPCSTR lpszSuggest, LPCVOID pIgnore = NULL) const;

	void EnumInstances(fnEnumInstProc lpfn, LPARAM lParam) const;
	void EnumSchedules(fnEnumSchProc lpfn, LPARAM lParam) const;
	void EditGlobalData();

	const CThreadList* GetThreadList() const;
	CSIGInstance* FindThread(LPCSTR lpszName);
	CSIGSchedule* FindSchedule(LPCSTR lpszName);
	BOOL EditDictionary();

	BOOL Start();
	BOOL Stop();

	
private:

	BOOL IsNameUnique(BOOL bThreadOrSchedule, LPCSTR lpszSuggest, LPCVOID pIgnore) const;
	static BOOL FindNumericSuffix(CString& str, int& num);
	void OnInstanceTick();
	void OnWndTick();

	static void CALLBACK WndProc(LPVOID lParam, LPCVBYTE lpStopFlag);
	static void CALLBACK InstanceProc(LPVOID lParam, LPCVBYTE lpStopFlag);

	CTickThread m_instanceTicker;
	CTickThread m_wndTicker;
	GLOBALDATA m_global;
	CStringArrayEx m_aGameNames;
	CRealmInfo m_ri;
	CThreadList m_aInstances;
	CScheduleList m_aSchedules;
	CString m_sPassword;
};

CSIGData* GlobalGetSIGData();

#endif // __SIGDATA_H__
