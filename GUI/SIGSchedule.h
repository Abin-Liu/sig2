//////////////////////////////////////////////////////////////////////
// SIGSchedule.h
//
// SIG schedule data configuration class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __SIGSCHEDULE_H__
#define __SIGSCHEDULE_H__

#include "SIGInstance.h"
#include "External\\LinkedList.h"
#include "External\\StreamBuffer.h"
#include "SIGDataDef.h"	// Added by ClassView

class CSIGTask;
class CSIGSchedule;

// Some typedef's to make the code less messy
typedef void (CALLBACK *fnTaskNotify)(CSIGTask* pTask, BOOL bStart, LPARAM lParam);
void CALLBACK FreeSchTask(CSIGTask*& pData);
typedef CLinkedList<CSIGTask*, CSIGTask*, FreeSchTask> CTaskList;
void CALLBACK FreeSchedule(CSIGSchedule*& pData);
typedef CLinkedList<CSIGSchedule*, CSIGSchedule*, FreeSchedule> CScheduleList;

typedef void (CALLBACK *fnEnumTaskProc)(const CScheduleData* pTask, LPARAM lParam);

class CSIGTask  
{
public:			

	CSIGTask(fnTaskNotify lpfn, LPARAM lParam);
	~CSIGTask();

	BOOL LoadData(CStreamBuffer& sb);
	void SaveData(CStreamBuffer &sb) const;
	BOOL IsValid() const;
	const CScheduleData* GetData() const;
	BOOL Prepare(const CThreadList* pList);
	void OnDeleteInstance(const CSIGInstance* pInstance);
	BOOL SetData(const CScheduleData* pData);

	void OnTimerTick();
	BOOL Start();
	void Stop();
	void OnPrevStop();
	void OnPrevStart();

private:

	void OnStart();
	void OnStop();
	void CheckInstance();
	
	CScheduleData m_data;
	CSIGInstance* m_pInstance;
	int m_nStartTick;
	const fnTaskNotify m_lpfn;
	const LPARAM m_lParam;
	BOOL m_bStarted;
};

class CSIGSchedule
{
public:		
	
	CSIGSchedule();
	~CSIGSchedule();

	BOOL LoadData(CStreamBuffer& sb);
	BOOL SaveData(CStreamBuffer &sb) const;
	void OnScheduleTick();
	void EnumTasks(fnEnumTaskProc lpfn, LPARAM lParam) const;	
	BOOL IsStarted() const;
	void SetName(LPCSTR lpszName);
	LPCSTR GetName() const;
	void OnDeleteInstance(const CSIGInstance *pInstance);
	void SetData(const CScheduleData* aData, int nCount);
	int GetRepeat() const;
	void SetRepeat(int nCount);

	BOOL Start(const CThreadList *pList);
	void Stop();
	
private:
	void OnAllTasksCompleted();
	void PostError(LPCSTR lpsz);
	void PostString(LPCSTR lpsz);
	int Prepare(const CThreadList *pList);
	CSIGTask* GetNextValidTask(CSIGTask* pTask);
	void OnTaskNotify(CSIGTask *pTask, BOOL bStart);
	static void CALLBACK TaskNotify(CSIGTask* pTask, BOOL bStart, LPARAM lParam);

	CString m_sName;
	int m_nRepeat;
	int m_nCurCount;
	CTaskList m_aTasks;
	BOOL m_bStarted;
	const CThreadList* m_pThreadList;
};

#endif // __SIGSCHEDULE_H__
