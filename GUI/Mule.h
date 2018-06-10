//////////////////////////////////////////////////////////////////////
// Mule.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __MULE_H__
#define __MULE_H__

#include <windows.h>
#include "SIGDataDef.h"
#include "External\\ArrayEx.h"

enum { MULE_NO_MORE = 0, MULE_SAME_ACCOUNT, MULE_DIFF_ACCOUNT, MULE_DIFF_REALM };

typedef CArrayEx<CMuleData, const CMuleData&> CMuleArray;

class CMule  
{
public:		
	BOOL Verify() const;

	CMule();
	virtual ~CMule();

	BOOL LoadMuleData(CStreamBuffer& sb, LPCSTR lpszPassword = NULL);
	BOOL SaveMuleData(CStreamBuffer& sb, LPCSTR lpszPassword = NULL) const;

	void SetGameLen(int nGameLen);
	int GetGameLen() const;
	CMuleArray* GetMuleArray();
	void Copy(const CMule* pMule);

	void Initialize();
	int GetNextLogin(CLoginData& rLogin, BOOL bMustChangeAccount = FALSE);

private:

	CMuleArray m_aData;
	int m_nCurIdx;
	int m_nGameLen;
};

#endif // __MULE_H__
