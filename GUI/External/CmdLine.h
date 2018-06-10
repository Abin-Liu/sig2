//////////////////////////////////////////////////////////////////////
// CmdLine.h
//
// Command line parameter parser class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include "ArrayEx.h"
#include "StringEx.h"

#define CMD_SWITCH_DASH		0x01 // dash ("-")
#define CMD_SWITCH_SLASH	0x02 // fore slash ("/")

typedef BOOL (CALLBACK *fnEnumCmdLineProc)(LPCTSTR lpszString, LPARAM lParam);

class CCmdLineItem;
class CCmdLine  
{
public:	

	///////////////////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////////////////
	CCmdLine();
	CCmdLine(const CCmdLine& rSrc);
	CCmdLine(DWORD dwSwitchFlags);
	CCmdLine(LPCTSTR lpszCmdLine, DWORD dwSwitchFlags = CMD_SWITCH_DASH | CMD_SWITCH_SLASH);
	CCmdLine(int argc, LPCTSTR* argv, DWORD dwSwitchFlags = CMD_SWITCH_DASH | CMD_SWITCH_SLASH);

	///////////////////////////////////////////////////////////////////////
	// Attributes & Operations
	///////////////////////////////////////////////////////////////////////
	LPCTSTR GetCmdLine() const;
	int SetCmdLine(LPCTSTR lpszCmdLine);
	int SetCmdLine(int argc, LPCTSTR* argv);
	DWORD GetSwitchFlags() const;
	void SetSwitchFlags(DWORD dwRemove, DWORD dwAdd);

	///////////////////////////////////////////////////////////////////////
	// Switches & Values Searching
	///////////////////////////////////////////////////////////////////////
	BOOL HasSwitch(LPCTSTR lpszSwitch) const;
	BOOL HasValue(LPCTSTR lpszSwitch, LPCTSTR lpszValue) const;
	int GetSwitchCount() const;	
	int GetSwitchValueCount(LPCTSTR lpszSwitch) const;
	LPCTSTR GetSafeValue(LPCTSTR lpszSwitch, LPCTSTR lpszDefault = NULL) const;

	///////////////////////////////////////////////////////////////////////
	// Enumerations
	///////////////////////////////////////////////////////////////////////
	BOOL EnumSwitches(fnEnumCmdLineProc lpfn, LPARAM lParam = 0) const;
	BOOL EnumSwitchValues(LPCTSTR lpszSwitch, fnEnumCmdLineProc lpfn, LPARAM lParam = 0) const;

	///////////////////////////////////////////////////////////////////////
	// Operator
	///////////////////////////////////////////////////////////////////////
	const CCmdLine& operator=(const CCmdLine& rhs);

private:	
	
	///////////////////////////////////////////////////////////////////////
	// Internal Methods
	///////////////////////////////////////////////////////////////////////
	BOOL IsSwitch(const CStringEx& rStr) const;
	static CStringEx GetSegment(CStringEx& sCmdLine);	

	///////////////////////////////////////////////////////////////////////
	// Member Data
	///////////////////////////////////////////////////////////////////////
	CStringEx m_sCmdLine;
	CArrayEx<CCmdLineItem, const CCmdLineItem&> m_aItems;
	DWORD m_dwSwitchFlags;
};


class CCmdLineItem
{
	friend class CCmdLine;

public:

	CCmdLineItem();
	CCmdLineItem(const CCmdLineItem& rSrc);
	const CCmdLineItem& operator=(const CCmdLineItem& rhs);

private:
	
	CStringEx m_sSwitch;
	CArrayEx<CStringEx, const CStringEx&> m_aValues;
};

#endif // __CMDLINE_H__
