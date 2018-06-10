//////////////////////////////////////////////////////////////////
// NameDictionary.h
//
// Game name dictionary class definition.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////

#ifndef __NAMEDICTIONARY_H__
#define __NAMEDICTIONARY_H__

#include <windows.h>
#include "SIGDataDef.h"
#include "External\\ArrayEx.h"

typedef CArrayEx<CString, const CString&> CStringArrayEx;

class CNameDictionary
{
public:		

	CNameDictionary();
	~CNameDictionary();


	CString GetGameName();
	CString GetGamePwd() const;	
	const CStringArrayEx* GetNameArray() const;
	void SetNameArray(const CStringArrayEx* pArray);
	LPCNAMERULE GetRule() const;
	void SetRule(LPCNAMERULE pRule);
	
private:

	CString GetRandomString(int nLen) const;
	
	NAMERULE m_rule;
	int m_nCurIdx;
	int m_nCurNameSuffix;
	const CStringArrayEx* m_pGameNames;
};

#endif // __NAMEDICTIONARY_H__