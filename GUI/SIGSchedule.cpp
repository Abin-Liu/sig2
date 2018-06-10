//////////////////////////////////////////////////////////////////////
// SIGSchedule.cpp
//
// SIG schedule data configuration class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIGSchedule.h"

void CALLBACK FreeSchTask(CSIGTask*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

void CALLBACK FreeSchedule(CSIGSchedule*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

CSIGTask::CSIGTask(fnTaskNotify lpfn, LPARAM lParam)
: m_lpfn(lpfn), m_lParam(lParam)
{
	ASSERT(m_lpfn);
	m_pInstance = NULL;
	m_nStartTick = -1;
	m_bStarted = FALSE;
}

CSIGTask::~CSIGTask()
{
	if (m_pInstance)
		m_pInstance->Stop(TRUE);
}

BOOL CSIGTask::LoadData(CStreamBuffer &sb)
{
	return m_data.LoadData(sb);
}

void CSIGTask::SaveData(CStreamBuffer &sb) const
{
	m_data.SaveData(sb);
}

BOOL CSIGTask::IsValid() const
{
	return m_pInstance && m_pInstance->Verify();
}

void CSIGTask::CheckInstance()
{
	if (m_pInstance == NULL)
		return;

	if (m_bStarted && !m_pInstance->IsStarted())
		OnStop();

	if (!m_bStarted && m_pInstance->IsStarted())
		OnStart();
}

void CSIGTask::OnStop()
{
	m_bStarted = FALSE;
	if (m_lpfn)
		m_lpfn(this, FALSE, m_lParam);
}

void CSIGTask::OnDeleteInstance(const CSIGInstance *pInstance)
{
	if (m_pInstance == NULL)
		return;

	if (m_pInstance == pInstance)
	{
		if (m_bStarted)
			OnStop();
		m_pInstance = NULL;
	}

	m_nStartTick = -1;
	m_bStarted = FALSE;
}

void CSIGTask::OnStart()
{
	m_bStarted = TRUE;
	if (m_lpfn)
		m_lpfn(this, TRUE, m_lParam);
}

BOOL CSIGTask::Start()
{
	if (m_pInstance == NULL)
		return FALSE;

	int nTimeLimit = -1;
	int nGameLimit = -1;

	if (m_data.nEndRule == SCH_END_LENGTH)
		nTimeLimit = m_data.nEndParam;
	else if (m_data.nEndRule == SCH_END_GAMES)
		nGameLimit = m_data.nEndParam;
	
	if (m_pInstance->IsStarted())
		m_pInstance->Stop(TRUE);

	return m_pInstance->Start(nTimeLimit, nGameLimit);
}

void CSIGTask::Stop()
{
	if (m_pInstance)
		m_pInstance->Stop(TRUE);
}

void CSIGTask::OnPrevStart()
{
	if (m_data.nStartRule == SCH_START_AFTERPREVSTART)
		m_nStartTick = max(1, m_data.nStartParam);
}

void CSIGTask::OnPrevStop()
{
	if (m_data.nStartRule == SCH_START_AFTERPREVSTOP)
		m_nStartTick = max(1, m_data.nStartParam);
}

void CSIGTask::OnTimerTick()
{
	if (m_pInstance == NULL)
		return;

	CheckInstance();

	if (m_nStartTick > 0 && !m_pInstance->IsStarted())
	{
		// shall we start now?
		m_nStartTick--;
		if (m_nStartTick == 0)
			Start();
	}
}

const CScheduleData* CSIGTask::GetData() const
{
	return &m_data;
}

BOOL CSIGTask::SetData(const CScheduleData* pData)
{
	if (pData == NULL)
		return FALSE;

	m_data = *pData;
	m_pInstance = NULL;
	return TRUE;
}

CSIGSchedule::CSIGSchedule()
{
	m_bStarted = FALSE;
	m_nRepeat = 0;
	m_nCurCount = 0;
	m_pThreadList = NULL;
}

CSIGSchedule::~CSIGSchedule()
{
}

BOOL CSIGSchedule::LoadData(CStreamBuffer& sb)
{
	m_nCurCount = 0;
	sb >> m_sName >> m_nRepeat;
	int nCount = 0;
	sb >> nCount;
	m_aTasks.Lock();
	m_aTasks.RemoveAll();

	for (int i = 0; i < nCount; i++)
	{
		CSIGTask* p = new CSIGTask(TaskNotify, (LPARAM)this);
		ASSERT(p);

		if (!p->LoadData(sb))
		{
			delete p;
			break;
		}
		else
		{
			m_aTasks.InsertRear(p);
		}
	}

	m_aTasks.Unlock();
	return TRUE;
}

BOOL CSIGSchedule::SaveData(CStreamBuffer &sb) const
{
	sb << m_sName << m_nRepeat;

	m_aTasks.Lock();
	sb << m_aTasks.GetItemCount();

	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		const CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		p->SaveData(sb);
	}
	m_aTasks.Unlock();
	return TRUE;
}


void CSIGSchedule::TaskNotify(CSIGTask *pTask, BOOL bStart, LPARAM lParam)
{
	CSIGSchedule* pSch = (CSIGSchedule*)lParam;
	if (pSch)
		pSch->OnTaskNotify(pTask, bStart);
}

void CSIGSchedule::OnTaskNotify(CSIGTask *pTask, BOOL bStart)
{
	if (!m_bStarted)
		return;

	BOOL bAllCompleted = FALSE;
	m_aTasks.Lock();
	CSIGTask* pNext = GetNextValidTask(pTask);
	if (pNext)
	{
		if (bStart)
			pNext->OnPrevStart();
		else
			pNext->OnPrevStop();
	}
	else
	{
		if (!bStart)
			bAllCompleted = TRUE;			
	}

	m_aTasks.Unlock();

	if (bAllCompleted)
		OnAllTasksCompleted();
}

CSIGTask* CSIGSchedule::GetNextValidTask(CSIGTask *pTask)
{
	// assume the caller has already locked the list
	if (pTask == NULL)
		return NULL;

	POSITION pos = m_aTasks.Find(pTask);
	if (pos == NULL)
		return NULL;

	while (pos)
	{
		CSIGTask* pNext = m_aTasks.GetNext(pos);
		if (pNext != pTask && pNext->IsValid())
			return pNext;
	}

	return NULL;
}

BOOL CSIGSchedule::Start(const CThreadList *pList)
{
	if (pList)
	{
		m_pThreadList = pList;
		::SendMessage(g_hViewWnd, WM_SCH_START, (WPARAM)this, 0);
	}

	if (m_pThreadList == NULL)
		return FALSE;
	
	if (m_aTasks.IsEmpty())
	{
		PostError(g_bLanguage ? "该计划包含0个任务" : "This schedule contains 0 tasks");
		return FALSE;
	}

	if (!Prepare(m_pThreadList))
	{
		PostError(g_bLanguage ? "该计划中所有的任务都不合法" : "This schedule contains none valid tasks");
		return FALSE;
	}
	
	m_bStarted = TRUE;
	BOOL bOK = FALSE;
	m_aTasks.Lock();
	BOOL bFirst = TRUE;
	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos && !bOK)
	{
		CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		if (p->IsValid() && (p->GetData()->nStartRule == SCH_START_NOW || bFirst))
			p->Start();
		bFirst = FALSE;			
	}
	m_aTasks.Unlock();
	return bOK;	
}

void CSIGSchedule::Stop()
{
	m_bStarted = FALSE;
	m_aTasks.Lock();
	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		p->Stop();
			
	}
	m_aTasks.Unlock();
	::SendMessage(g_hViewWnd, WM_SCH_STOP, (WPARAM)this, 0);
}

void CSIGSchedule::OnScheduleTick()
{
	if (!m_bStarted)
		return;

	m_aTasks.Lock();
	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		p->OnTimerTick();
			
	}
	m_aTasks.Unlock();
}

void CSIGSchedule::EnumTasks(fnEnumTaskProc lpfn, LPARAM lParam) const
{
	if (lpfn == NULL)
		return;

	m_aTasks.Lock();
	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		const CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		lpfn(p->GetData(), lParam);
	}

	m_aTasks.Unlock();
}

void CSIGSchedule::SetData(const CScheduleData* aData, int nCount)
{
	Stop();
	m_aTasks.Lock();
	m_aTasks.RemoveAll();

	if (aData && nCount > 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			CSIGTask* p = new CSIGTask(TaskNotify, (LPARAM)this);
			ASSERT(p);

			if (!p->SetData(&aData[i]))
				delete p;
			else
				m_aTasks.InsertRear(p);
		}
	}	

	m_aTasks.Unlock();
}

void CSIGSchedule::OnDeleteInstance(const CSIGInstance *pInstance)
{
	m_aTasks.Lock();
	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		p->OnDeleteInstance(pInstance);
	}
	m_aTasks.Unlock();
}

LPCSTR CSIGSchedule::GetName() const
{
	return m_sName;
}

void CSIGSchedule::SetName(LPCSTR lpszName)
{
	if (lpszName == NULL || lpszName[0] == 0)
		return;

	m_sName = lpszName;
}

BOOL CSIGSchedule::IsStarted() const
{
	return m_bStarted;
}

BOOL CSIGTask::Prepare(const CThreadList *pList)
{
	if (pList == NULL)
		return FALSE;

	Stop();
	m_pInstance = NULL;

	// assume the caller already locked this list
	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		CSIGInstance* p = pList->GetNext(pos);
		ASSERT(p);
		if (p->Verify() && ::stricmp(p->GetName(), m_data.sThreadName) == 0)
		{
			m_pInstance = p;
			return TRUE;
		}
	}

	return FALSE;
}

int CSIGSchedule::Prepare(const CThreadList *pList)
{
	if (pList == NULL)
		return 0;

	int nCount = 0;

	pList->Lock();
	m_aTasks.Lock();

	POSITION pos = m_aTasks.GetHeadPosition();
	while (pos)
	{
		CSIGTask* p = m_aTasks.GetNext(pos);
		ASSERT(p);
		if (p->Prepare(pList))
			nCount++;
	}

	m_aTasks.Unlock();
	pList->Unlock();

	return nCount;
}

void CSIGSchedule::PostString(LPCSTR lpsz)
{
	::SendMessage(g_hViewWnd, WM_SCH_STRING, (WPARAM)this, (LPARAM)lpsz);
}

void CSIGSchedule::PostError(LPCSTR lpsz)
{
	Stop();
	::SendMessage(g_hViewWnd, WM_SCH_ERROR, (WPARAM)this, (LPARAM)lpsz);
}

void CSIGSchedule::SetRepeat(int nCount)
{
	m_nRepeat = nCount;
	m_nCurCount = 0;
}

int CSIGSchedule::GetRepeat() const
{
	return m_nRepeat;
}

void CSIGSchedule::OnAllTasksCompleted()
{
	m_nCurCount++;
	if (m_nRepeat > 0 && m_nCurCount >= m_nRepeat)
		Stop();
	else
		Start(NULL);
}
