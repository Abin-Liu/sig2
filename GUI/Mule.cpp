//////////////////////////////////////////////////////////////////////
// Mule.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mule.h"

CMule::CMule()
{
	m_nCurIdx = 0;
	m_nGameLen = 10;
}

CMule::~CMule()
{

}

BOOL CMule::LoadMuleData(CStreamBuffer& sb, LPCSTR lpszPassword)
{	
	m_nGameLen = 10;
	sb >> m_nGameLen;

	int nCount = 0;
	sb >> nCount;
	
	m_aData.Lock();
	m_aData.RemoveAll();
	m_nCurIdx = 0;

	for (int i = 0; i < nCount; i++)
	{
		CMuleData md;
		md.LoadData(sb, lpszPassword);
		md.nCurIdx = 0;
		m_aData.Add(md);
	}	

	Initialize();
	
	m_aData.Unlock();
	return m_aData.GetSize();
}

BOOL CMule::SaveMuleData(CStreamBuffer& sb, LPCSTR lpszPassword) const
{
	sb << m_nGameLen;

	m_aData.Lock();
	int nCount = m_aData.GetSize();
	sb << nCount;

	for (int i = 0; i < nCount; i++)
		m_aData[i].SaveData(sb, lpszPassword);

	m_aData.Unlock();
	return nCount;
}

int CMule::GetNextLogin(CLoginData& rLogin, BOOL bMustChangeAccount)
{
	if (!m_aData.IsIndexValid(m_nCurIdx))
		return MULE_NO_MORE;

	CString sBnetIP = rLogin.sBnetIP;
	CString sSubRealm = rLogin.sSubRealm;

	if (!bMustChangeAccount && m_aData[m_nCurIdx].CheckLogin(rLogin))
		return MULE_SAME_ACCOUNT;

	for (int i = m_nCurIdx + 1; i < m_aData.GetSize(); i++)
	{
		if (m_aData[i].CheckLogin(rLogin))
		{
			m_nCurIdx = i;
			if (sBnetIP == rLogin.sBnetIP && sSubRealm == rLogin.sSubRealm)
				return MULE_DIFF_ACCOUNT;
			else
				return MULE_DIFF_REALM;
		}
	}

	m_nCurIdx = 256;
	return MULE_NO_MORE;
}

void CMule::Initialize()
{
	m_nCurIdx = 0;
	for (int i = 0; i < m_aData.GetSize(); i++)
		m_aData[i].nCurIdx = 0;
}

CMuleArray* CMule::GetMuleArray()
{
	return &m_aData;
}

int CMule::GetGameLen() const
{
	return max(5, m_nGameLen);
}

void CMule::SetGameLen(int nGameLen)
{
	m_nGameLen = max(nGameLen, 5);
}

void CMule::Copy(const CMule *pMule)
{
	if (pMule)
	{
		m_aData.Lock();
		m_aData.Copy(pMule->m_aData);
		m_nCurIdx = pMule->m_nCurIdx;
		m_nGameLen = pMule->m_nGameLen;
		m_aData.Unlock();
	}
}

BOOL CMule::Verify() const
{
	if (m_aData.IsEmpty())
		return FALSE;

	BOOL bOK = TRUE;
	m_aData.Lock();
	for (int i = 0; i < m_aData.GetSize(); i++)
	{
		if (!m_aData[i].Verify())
		{
			bOK = FALSE;
			break;
		}
	}
		
	m_aData.Unlock();
	return bOK;
}
