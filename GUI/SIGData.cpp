#include "stdafx.h"
#include "sig2.h"
#include "SIGData.h"
#include "SIGConfigSheet.h"
#include "SchSheet.h"
#include "GlobalDlg.h"
#include "External\\StreamBuffer.h"
#include "External\\Encrypt.h"
#include "DicDlg.h"

const DWORD SAVE_VER = 0x20050213;

LPCVOID g_pCurrentLaunchingInst = NULL;
CSIGData* g_pSIGData = NULL;

CSIGData* GlobalGetSIGData()
{
	return g_pSIGData;
}


CSIGData::CSIGData()
{	
	g_pSIGData = this;
	SetDefaultGlobalData(&m_global);
}

CSIGData::~CSIGData()
{
	Stop();
}

int CSIGData::LoadData(LPCSTR lpszFile, LPCSTR lpszDicFile, LPCSTR lpszRealms)
{
	m_sPassword = AfxGetApp()->GetProfileString("", "User ID", "<empty userid>");
	m_ri.InitPrivateRealms(lpszRealms);
	SetDefaultGlobalData(&m_global);

	CStreamBuffer sb;
	if (!sb.ReadFromFile(lpszFile) || sb.GetLength() < 8 + sizeof(m_global))
		return 0;

	DWORD dwCrc32 = 0;
	sb >> dwCrc32;

	if (dwCrc32 == 0 || dwCrc32 != CalcCRC32(sb.GetDataPtr(4), sb.GetLength(4)))
		return 0;

	sb >> dwCrc32;
	if (dwCrc32 != SAVE_VER)
		return 0;

	sb >> dwCrc32;

	BOOL bUserIDChanged = FALSE;	
	if (CalcCRC32((LPCSTR)m_sPassword, m_sPassword.GetLength()) != dwCrc32)
	{
		bUserIDChanged = TRUE;
		DWORD dwTick = ::GetTickCount();
		char szPwd[64] = "";
		CalcMD5(&dwTick, 4, szPwd, 63);
		m_sPassword = szPwd;
		AfxGetApp()->WriteProfileString("", "User ID", m_sPassword);
	}

	// Load global data
	sb.StreamRead(&m_global, sizeof(m_global));

	// load config data	
	int i = 0;
	int nCount = 0;
	sb >> nCount;	

	m_aInstances.Lock();
	for (i = 0; i < nCount; i++)
	{
		CSIGInstance* p = new CSIGInstance(&m_global, &m_aGameNames);
		ASSERT(p);
		p->LoadData(sb, m_sPassword);
		m_aInstances.InsertRear(p);
	}
	m_aInstances.Unlock();

	// load schedules
	sb >> nCount;
	m_aSchedules.Lock();
	m_aSchedules.RemoveAll();

	for (i = 0; i < nCount; i++)
	{
		CSIGSchedule* p = new CSIGSchedule;
		ASSERT(p);
		p->LoadData(sb);
		m_aSchedules.InsertRear(p);
	}

	m_aSchedules.Unlock();

	// Load game name dictionary
	m_aGameNames.Lock();
	m_aGameNames.RemoveAll();
	CStdioFile file;
	if (file.Open(lpszDicFile, CFile::modeRead))
	{
		CString str;
		while (file.ReadString(str))
		{
			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty())
				m_aGameNames.Add(str);
		}

		file.Close();
	}
	m_aGameNames.Unlock();	

	sb.Clear();

	if (bUserIDChanged && m_aInstances.GetItemCount())
		return -1;

	return 1;
}

BOOL CSIGData::SaveData(LPCSTR lpszFile, LPCSTR lpszDicFile, LPCSTR lpszRealms) const
{
	m_ri.SavePrivateRealms(lpszRealms);
	
	CStreamBuffer sb;	

	// place holder for crc32
	DWORD dwCrc32 = 0;
	sb << dwCrc32 << SAVE_VER << dwCrc32;

	// save global data
	sb.Append(&m_global, sizeof(GLOBALDATA));

	// save config date including mule mode
	m_aInstances.Lock();
	sb << m_aInstances.GetItemCount();

	POSITION pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		const CSIGInstance* p = m_aInstances.GetNext(pos);
		ASSERT(p);
		p->SaveData(sb, m_sPassword);
	}
	m_aInstances.Unlock();

	// save schedules
	m_aSchedules.Lock();
	sb << m_aSchedules.GetItemCount();

	pos = m_aSchedules.GetHeadPosition();
	while (pos)
	{
		const CSIGSchedule* p = m_aSchedules.GetNext(pos);
		ASSERT(p);
		p->SaveData(sb);
	}
	m_aSchedules.Unlock();

	// save game name dictionary
	CStdioFile file;
	if (file.Open(lpszDicFile, CFile::modeWrite | CFile::modeCreate))
	{
		m_aGameNames.Lock();
		for (int i = 0; i < m_aGameNames.GetSize(); i++)
		{
			file.WriteString(m_aGameNames[i]);
			file.WriteString("\n");
		}
		m_aGameNames.Unlock();
		file.Close();
	}	

	// calculate crc32
	dwCrc32 = CalcCRC32((LPCSTR)m_sPassword, m_sPassword.GetLength());
	sb.Overwrite(&dwCrc32, 4, 8);

	dwCrc32 = CalcCRC32(sb.GetDataPtr(4), sb.GetLength(4));
	sb.Overwrite(&dwCrc32, 4, 0);
	
	return sb.WriteToFile(lpszFile);
}

void CSIGData::InstanceProc(LPVOID lParam, LPCVBYTE lpStopFlag)
{
	CSIGData* pData = (CSIGData*)lParam;
	if (pData)
		pData->OnInstanceTick();
}

void CSIGData::WndProc(LPVOID lParam, LPCVBYTE lpStopFlag)
{
	CSIGData* pData = (CSIGData*)lParam;
	if (pData)
		pData->OnWndTick();
}

void CSIGData::OnInstanceTick()
{
	int nThreads = 0;
	int nSchedules = 0;
	int nThreadRunning = 0;
	int nScheduleRunning = 0;

	m_aInstances.Lock();
	nThreads = m_aInstances.GetItemCount();
	POSITION pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		CSIGInstance* p = m_aInstances.GetNext(pos);
		if (p)
		{
			p->OnInstanceTick();
			if (p->IsStarted())
				nThreadRunning++;
		}
	}
	m_aInstances.Unlock();

	m_aSchedules.Lock();
	pos = m_aSchedules.GetHeadPosition();
	nSchedules = m_aSchedules.GetItemCount();
	while (pos)
	{
		CSIGSchedule* p = m_aSchedules.GetNext(pos);
		if (p)
		{
			p->OnScheduleTick();
			if (p->IsStarted())
				nScheduleRunning++;
		}
	}
	m_aSchedules.Unlock();

	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd && ::IsWindow(pWnd->m_hWnd))
	{
		pWnd->PostMessage(WM_THREAD_COUNT, nThreadRunning, nThreads);
		pWnd->PostMessage(WM_SCHEDULE_COUNT, nScheduleRunning, nSchedules);
	}
}

void CSIGData::OnWndTick()
{
	m_aInstances.Lock();
	POSITION pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		CSIGInstance* p = m_aInstances.GetNext(pos);
		if (p)
			p->OnWndTick();
	}
	m_aInstances.Unlock();
}

BOOL CSIGData::Start()
{
	return m_instanceTicker.Start(InstanceProc, (LPVOID)this, INSTANCE_TICK)
		&& m_wndTicker.Start(WndProc, (LPVOID)this, WND_TICK);
}

BOOL CSIGData::Stop()
{
	m_aSchedules.Lock();
	POSITION pos = m_aSchedules.GetHeadPosition();
	while (pos)
	{
		CSIGSchedule* p = m_aSchedules.GetNext(pos);
		if (p)
			p->Stop();
	}
	m_aSchedules.Unlock();

	m_aInstances.Lock();
	pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		CSIGInstance* p = m_aInstances.GetNext(pos);
		if (p)
			p->Stop(TRUE);
	}
	m_aInstances.Unlock();	

	return m_instanceTicker.Stop(3000) && m_wndTicker.Stop(2000);
}

const CSIGInstance* CSIGData::CreateInstance()
{
	CSIGConfigSheet sheet("", AfxGetMainWnd());
	sheet.Initialize(FALSE, this, &m_ri, NULL);
	if (sheet.DoModal() != ID_WIZFINISH)
		return NULL;

	CSIGThreadData* pNewData = sheet.GetThreadData();
	ASSERT(pNewData);
	
	pNewData->sName = GetUniqueName(TRUE, pNewData->sName, NULL);

	CSIGInstance* pInstance = new CSIGInstance(&m_global, &m_aGameNames, sheet.GetMule() != NULL);
	ASSERT(pInstance);
	
	pInstance->SetData(pNewData);
	if (sheet.GetMule() != NULL)
		pInstance->SetMule(sheet.GetMule());

	m_aInstances.Lock();
	m_aInstances.InsertRear(pInstance);
	m_aInstances.Unlock();
	return pInstance;
}

BOOL CSIGData::EditInstance(CSIGInstance* pInstance)
{
	if (pInstance == NULL)
		return FALSE;
	
	CSIGConfigSheet sheet("SIG Thread Configuration", AfxGetMainWnd());
	sheet.Initialize(TRUE, this, &m_ri, pInstance);

	if (sheet.DoModal() != ID_WIZFINISH)
		return FALSE;

	CSIGThreadData* p = sheet.GetThreadData();
	ASSERT(p);
	p->sName = GetUniqueName(TRUE, p->sName, pInstance);

	pInstance->SetData(p);
	pInstance->SetMule(sheet.GetMule());

	return TRUE;
}

void CSIGData::EditGlobalData()
{
	CGlobalDlg dlg(&m_global);	
	if (dlg.DoModal() != IDOK)
		return;	

	::memcpy(&m_global, dlg.GetGlobal(), sizeof(m_global));
}

BOOL CSIGData::FindNumericSuffix(CString &str, int& num)
{
	num = 0;
	if (str.IsEmpty())
		return FALSE;

	if (str[str.GetLength() - 1] != ')')
		return FALSE;

	int n = str.ReverseFind('(');
	if (n < 1)
		return FALSE;

	CString sNum = str.Mid(n + 1);
	sNum.TrimRight(')');
	sNum.TrimLeft();
	sNum.TrimRight();

	if (sNum.IsEmpty())
	{
		str.Delete(n, str.GetLength() - n);
		return TRUE;
	}

	for (int i = 0; i < sNum.GetLength(); i++)
	{
		if (!::isdigit(sNum[i]))
			return FALSE;
	}

	str.Delete(n, str.GetLength() - n);
	num = atoi(sNum);
	return TRUE;
}

BOOL CSIGData::DeleteInstance(CSIGInstance* pInstance)
{
	if (pInstance == NULL)
		return FALSE;

	pInstance->Stop(TRUE);

	m_aSchedules.Lock();
	POSITION pos = m_aSchedules.GetHeadPosition();
	while (pos)
	{
		CSIGSchedule* p = m_aSchedules.GetNext(pos);
		ASSERT(p);
		p->OnDeleteInstance(pInstance);
	}
	m_aSchedules.Unlock();

	m_aInstances.Lock();
	pos = m_aInstances.Find(pInstance);
	BOOL bFound = pos != NULL;
	if (pos)
		m_aInstances.RemoveAt(pos);
	m_aInstances.Unlock();
	return bFound;
}

void CSIGData::EnumInstances(fnEnumInstProc lpfn, LPARAM lParam) const
{
	if (lpfn == NULL)
		return;

	m_aInstances.Lock();
	POSITION pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		const CSIGInstance* p = m_aInstances.GetNext(pos);
		ASSERT(p);
		lpfn(p, lParam);
	}
	m_aInstances.Unlock();
}

void CSIGData::EnumSchedules(fnEnumSchProc lpfn, LPARAM lParam) const
{
	if (lpfn == NULL)
		return;

	m_aSchedules.Lock();
	POSITION pos = m_aSchedules.GetHeadPosition();
	while (pos)
	{
		const CSIGSchedule* p = m_aSchedules.GetNext(pos);
		ASSERT(p);
		lpfn(p, lParam);
	}
	m_aSchedules.Unlock();
}

const CSIGSchedule* CSIGData::CreateSchedule()
{
	CSchSheet sheet("SIG Schedule Configuration");
	sheet.SetData(this, GetUniqueName(FALSE, "New SIG Schedule"));
	if (sheet.DoModal() != ID_WIZFINISH)
		return NULL;

	CSIGSchedule* p = new CSIGSchedule;
	ASSERT(p);
	p->SetName(GetUniqueName(FALSE, sheet.GetName()));
	p->SetRepeat(sheet.GetRepeat());

	int nCount = 0;
	const CScheduleData* aData = sheet.GetData(nCount);
	p->SetData(aData, nCount);
	m_aSchedules.Lock();
	m_aSchedules.InsertRear(p);
	m_aSchedules.Unlock();
	
	return p;
}

CString CSIGData::GetUniqueName(BOOL bThreadOrSchedule, LPCSTR lpszSuggest, LPCVOID pIgnore) const
{
	if (lpszSuggest == NULL || lpszSuggest[0] == 0)
		return "";

	int nSuffix = 0;
	if (IsNameUnique(bThreadOrSchedule, lpszSuggest, pIgnore))
		return lpszSuggest;

	CString sName(lpszSuggest);
	int nNum = 0;

	if (pIgnore == NULL)
		FindNumericSuffix(sName, nNum);

	CString sTry;

	do
	{
		nNum++;
		sTry.Format("%s(%d)", (LPCSTR)sName, nNum);
	} while (!IsNameUnique(bThreadOrSchedule, sTry, pIgnore));	

	return sTry;

}

BOOL CSIGData::IsNameUnique(BOOL bThreadOrSchedule, LPCSTR lpszSuggest, LPCVOID pIgnore) const
{
	if (lpszSuggest == NULL)
		return TRUE;

	BOOL bUnique = TRUE;
	if (bThreadOrSchedule)
	{
		m_aInstances.Lock();
	
		POSITION pos = m_aInstances.GetHeadPosition();
		while (pos)
		{
			const CSIGInstance* p = m_aInstances.GetNext(pos);
			ASSERT(p);

			if (pIgnore != p &&  ::stricmp(lpszSuggest, p->GetData()->sName) == 0)
			{
				bUnique = FALSE;
				break;
			}
		}

		m_aInstances.Unlock();
	}
	else
	{
		m_aSchedules.Lock();

		POSITION pos = m_aSchedules.GetHeadPosition();
		while (pos)
		{
			const CSIGSchedule* p = m_aSchedules.GetNext(pos);
			ASSERT(p);

			if (pIgnore != p &&  ::stricmp(lpszSuggest, p->GetName()) == 0)
			{
				bUnique = FALSE;
				break;
			}
		}

		m_aSchedules.Unlock();
	}
	
	return bUnique;
}

BOOL CSIGData::EditSchedule(CSIGSchedule *pSchedule)
{
	if (pSchedule == NULL)
		return FALSE;

	pSchedule->Stop();

	CSchSheet sheet("SIG Schedule Configuration");
	sheet.SetData(this, pSchedule->GetName(), pSchedule->GetRepeat(), pSchedule);
	if (sheet.DoModal() != ID_WIZFINISH)
		return NULL;

	pSchedule->SetName(GetUniqueName(FALSE, sheet.GetName(), pSchedule));
	pSchedule->SetRepeat(sheet.GetRepeat());

	int nCount = 0;
	const CScheduleData* aData = sheet.GetData(nCount);
	pSchedule->SetData(aData, nCount);
	return TRUE;
}

BOOL CSIGData::DeleteSchedule(CSIGSchedule *pSchedule)
{
	if (pSchedule == NULL)
		return FALSE;

	pSchedule->Stop();
	m_aSchedules.Lock();

	POSITION pos = m_aSchedules.Find(pSchedule);
	BOOL bFound = pos != NULL;
	m_aSchedules.RemoveAt(pos);
	m_aSchedules.Unlock();

	return bFound;
}

const CThreadList* CSIGData::GetThreadList() const
{
	return &m_aInstances;
}

int CSIGData::GetThreadCount(int *pRunning) const
{
	int nCount = 0;
	m_aInstances.Lock();
	nCount = m_aInstances.GetItemCount();
	if (pRunning)
	{
		*pRunning = 0;
		POSITION pos = m_aInstances.GetHeadPosition();
		while (pos)
		{
			const CSIGInstance* p = m_aInstances.GetNext(pos);
			if (p && p->IsStarted())
				*pRunning++;
		}
	}
	m_aInstances.Unlock();
	return nCount;
}

int CSIGData::GetScheduleCount(int *pRunning) const
{
	int nCount = 0;
	m_aSchedules.Lock();
	nCount = m_aSchedules.GetItemCount();
	if (pRunning)
	{
		*pRunning = 0;
		POSITION pos = m_aSchedules.GetHeadPosition();
		while (pos)
		{
			const CSIGSchedule* p = m_aSchedules.GetNext(pos);
			if (p && p->IsStarted())
				*pRunning++;
		}
	}
	m_aSchedules.Unlock();
	return nCount;
}

BOOL CSIGData::EditDictionary()
{
	CDicDlg dlg(&m_aGameNames);
	if (dlg.DoModal() != IDOK)
		return FALSE;

	m_aGameNames.Lock();
	m_aGameNames.Copy(*dlg.GetDictionary());
	m_aGameNames.Unlock();
	return TRUE;
}

CSIGSchedule* CSIGData::FindSchedule(LPCSTR lpszName)
{
	if (lpszName == NULL || lpszName[0] == 0)
		return NULL;

	CSIGSchedule* pRes = NULL;
	m_aSchedules.Lock();
	POSITION pos = m_aSchedules.GetHeadPosition();
	while (pos)
	{
		CSIGSchedule* p = m_aSchedules.GetNext(pos);
		if (p && !::stricmp(lpszName, p->GetName()))
		{
			pRes = p;
			break;
		}
	}
	m_aSchedules.Unlock();
	return pRes;
}

CSIGInstance* CSIGData::FindThread(LPCSTR lpszName)
{
	if (lpszName == NULL || lpszName[0] == 0)
		return NULL;

	CSIGInstance* pRes = NULL;
	m_aInstances.Lock();
	POSITION pos = m_aInstances.GetHeadPosition();
	while (pos)
	{
		CSIGInstance* p = m_aInstances.GetNext(pos);
		if (p && !::stricmp(lpszName, p->GetName()))
		{
			pRes = p;
			break;
		}
	}
	m_aInstances.Unlock();
	return pRes;
}
