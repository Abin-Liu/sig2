//////////////////////////////////////////////////////////////////
// NameDictionary.cpp
//
// Game name dictionary class implementation.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "NameDictionary.h"
#include <stdlib.h>

static const char NAMES[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

CNameDictionary::CNameDictionary()
{	
	::memset(&m_rule, 0, sizeof(m_rule));
	m_rule.nRule = NAME_RANDOM;
	m_nCurIdx = 0;
	m_nCurNameSuffix = 0;
	m_pGameNames = NULL;
}

CNameDictionary::~CNameDictionary()
{
}

CString CNameDictionary::GetGameName()
{
	::srand(::GetTickCount());

	m_pGameNames->Lock();
	BOOL bDicValid = m_pGameNames && m_pGameNames->GetSize();
	CString str;	
	
	if (m_rule.nRule == NAME_PREFIX && m_rule.szNamePrefix[0])
	{
		str.Format("%s%d", m_rule.szNamePrefix, ++m_nCurNameSuffix);
	}
	else if (m_rule.nRule == NAME_DICRANDOM && bDicValid)
	{
		int nIdx = rand() % m_pGameNames->GetSize();
		str = m_pGameNames->GetAt(nIdx);
	}
	else if (m_rule.nRule == NAME_DICTURN && bDicValid)
	{
		if (!m_pGameNames->IsIndexValid(m_nCurIdx))
			m_nCurIdx = 0;

		str = m_pGameNames->GetAt(m_nCurIdx++);
	}
	else
	{
		str = GetRandomString(5 + rand() % 10);
	}

	m_pGameNames->Unlock();
	return str;
}

CString CNameDictionary::GetGamePwd() const
{
	if (m_rule.bUseFixedPwd)
		return m_rule.szFixedPwd;
	
	return GetRandomString(1 + (rand() % 5));
}

CString CNameDictionary::GetRandomString(int nLen) const
{
	const int LIMIT = ::strlen(NAMES);
	ASSERT(LIMIT);
	nLen = min(15, nLen);
	char sz[16] = "";

	for (int i = 0; i < nLen; i++)
		sz[i] = NAMES[rand() % LIMIT];
	return sz;
}

void CNameDictionary::SetRule(LPCNAMERULE pRule)
{
	if (pRule)
		::memcpy(&m_rule, pRule, sizeof(m_rule));

	m_nCurIdx = 1;
	m_nCurNameSuffix = 0;
}

LPCNAMERULE CNameDictionary::GetRule() const
{
	return &m_rule;
}

const CStringArrayEx* CNameDictionary::GetNameArray() const
{
	return m_pGameNames;
}

void CNameDictionary::SetNameArray(const CStringArrayEx *pArray)
{
	m_pGameNames = pArray;
}
