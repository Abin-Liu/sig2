//////////////////////////////////////////////////////////////////////
// CmdLine.cpp
//
// Command line parameter parser class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <tchar.h>
#include <string.h>
#include <assert.h>
#include "CmdLine.h"

#define CHAR_QUOTE	_T('\"')
#define BLANKS		_T(" \t\r\n")
static const TCHAR EMPTY_STR[] = _T("");

//////////////////////////////////////////////////////////////////////
// CCmdLineItem Implementation
//////////////////////////////////////////////////////////////////////
CCmdLineItem::CCmdLineItem()
{
}

CCmdLineItem::CCmdLineItem(const CCmdLineItem& rSrc)
{
	m_sSwitch = rSrc.m_sSwitch;
	m_aValues.Copy(rSrc.m_aValues);
}

const CCmdLineItem& CCmdLineItem::operator=(const CCmdLineItem& rhs)
{
	if (this != &rhs)
	{
		m_sSwitch = rhs.m_sSwitch;
		m_aValues.Copy(rhs.m_aValues);
	}	

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CCmdLine Implementation
//////////////////////////////////////////////////////////////////////

CCmdLine::CCmdLine()
{
	m_dwSwitchFlags = CMD_SWITCH_DASH | CMD_SWITCH_SLASH;
}

CCmdLine::CCmdLine(const CCmdLine& rSrc)
{
	m_dwSwitchFlags = rSrc.m_dwSwitchFlags;
	m_aItems.Copy(rSrc.m_aItems);
}

CCmdLine::CCmdLine(DWORD dwSwitchFlags)
{
	m_dwSwitchFlags = dwSwitchFlags;
}

CCmdLine::CCmdLine(LPCTSTR lpszCmdLine, DWORD dwSwitchFlags)
{
	m_dwSwitchFlags = dwSwitchFlags;
	SetCmdLine(lpszCmdLine);
}

CCmdLine::CCmdLine(int argc, LPCTSTR* argv, DWORD dwSwitchFlags)
{
	m_dwSwitchFlags = dwSwitchFlags;
	SetCmdLine(argc, argv);
}

int CCmdLine::SetCmdLine(int argc, LPCTSTR *argv)
{
	CStringEx sCmdLine;
	for (int i = 0; i < argc && argv[i]; i++)
	{
		sCmdLine += argv[i];
		sCmdLine += CHAR_QUOTE;
	}

	return SetCmdLine(sCmdLine);
}

int CCmdLine::SetCmdLine(LPCTSTR lpszCmdLine)
{
	m_sCmdLine = CStringEx(lpszCmdLine);

	m_aItems.RemoveAll();
	if (lpszCmdLine == NULL || lpszCmdLine[0] == 0)
		return 0;

	CStringEx sCmdLine(lpszCmdLine);
	sCmdLine.Trim();
	if (sCmdLine.IsEmpty())
		return 0;

	CCmdLineItem item;
	
	while (!sCmdLine.IsEmpty())
	{
		CStringEx str = GetSegment(sCmdLine);
		if (IsSwitch(str))
		{
			if (item.m_sSwitch.GetLength())
				m_aItems.Add(item);

			item.m_sSwitch = str;
			item.m_aValues.RemoveAll();
		}
		else
		{
			item.m_aValues.Add(str);
		}
	}

	if (item.m_sSwitch.GetLength())
		m_aItems.Add(item);

	return m_aItems.GetSize();
}

CStringEx CCmdLine::GetSegment(CStringEx &sCmdLine)
{
	CStringEx str;
	sCmdLine.Trim();
	if (sCmdLine.IsEmpty())
		return _T("");

	BOOL bQuote = sCmdLine[0] == CHAR_QUOTE;
	int nIdx = bQuote ? sCmdLine.Find(CHAR_QUOTE, 1) : sCmdLine.FindOneOf(BLANKS);
	if (nIdx == -1)
	{
		str = sCmdLine;
		sCmdLine.Empty();
	}
	else
	{
		str = sCmdLine.Left(nIdx);
		sCmdLine.Delete(0, nIdx + 1);
	}

	if (bQuote)
		str.Trim(CHAR_QUOTE);
	else
		str.Trim();
	
	return str;
}

BOOL CCmdLine::IsSwitch(const CStringEx &rStr) const
{
	if (rStr.IsEmpty())
		return FALSE;

	return (rStr[0] == _T('-') && (m_dwSwitchFlags & CMD_SWITCH_DASH))
		|| (rStr[0] == _T('/') && (m_dwSwitchFlags & CMD_SWITCH_SLASH));
}

void CCmdLine::SetSwitchFlags(DWORD dwRemove, DWORD dwAdd)
{
	m_dwSwitchFlags &= ~dwRemove;
	m_dwSwitchFlags |= dwAdd;
}

DWORD CCmdLine::GetSwitchFlags() const
{
	return m_dwSwitchFlags;
}

int CCmdLine::GetSwitchCount() const
{
	return m_aItems.GetSize();
}

BOOL CCmdLine::HasSwitch(LPCTSTR lpszSwitch) const
{
	if (lpszSwitch == NULL || lpszSwitch[0] == 0)
		return FALSE;

	for (int i = 0; i < m_aItems.GetSize(); i++)
	{
		if (m_aItems[i].m_sSwitch.CompareNoCase(lpszSwitch) == 0)
			return (BOOL)&m_aItems[i];
	}

	return FALSE;
}

BOOL CCmdLine::HasValue(LPCTSTR lpszSwitch, LPCTSTR lpszValue) const
{
	const CCmdLineItem* p = (const CCmdLineItem*)HasSwitch(lpszSwitch);
	if (p == NULL)
		return FALSE;

	if (lpszValue == NULL || lpszValue[0] == 0)
		return FALSE;

	const int IDX = p->m_aValues.Find(lpszValue);
	if (IDX == -1)
		return FALSE;

	return (BOOL)&p->m_aValues[IDX];
}

BOOL CCmdLine::EnumSwitches(fnEnumCmdLineProc lpfn, LPARAM lParam) const
{
	if (lpfn == NULL)
		return FALSE;

	for (int i = 0; i < m_aItems.GetSize(); i++)
	{
		if (!lpfn(m_aItems[i].m_sSwitch, lParam))
			return FALSE;
	}

	return TRUE;
}

BOOL CCmdLine::EnumSwitchValues(LPCTSTR lpszSwitch, fnEnumCmdLineProc lpfn, LPARAM lParam) const
{
	if (lpfn == NULL)
		return FALSE;

	const CCmdLineItem* p = (const CCmdLineItem*)HasSwitch(lpszSwitch);
	if (p == NULL)
		return FALSE;

	for (int i = 0; i < p->m_aValues.GetSize(); i++)
	{
		if (!lpfn(p->m_aValues[i], lParam))
			return FALSE;
	}

	return TRUE;
}

const CCmdLine& CCmdLine::operator=(const CCmdLine& rhs)
{
	if (this != &rhs)
	{
		m_dwSwitchFlags = rhs.m_dwSwitchFlags;
		m_aItems.Copy(rhs.m_aItems);
	}
	
	return *this;
}

LPCTSTR CCmdLine::GetCmdLine() const
{
	return m_sCmdLine;
}

int CCmdLine::GetSwitchValueCount(LPCTSTR lpszSwitch) const
{
	const CCmdLineItem* p = (const CCmdLineItem*)HasSwitch(lpszSwitch);
	if (p == NULL)
		return FALSE;

	return p->m_aValues.GetSize();
}

LPCTSTR CCmdLine::GetSafeValue(LPCTSTR lpszSwitch, LPCTSTR lpszDefault) const
{
	const CCmdLineItem* p = (const CCmdLineItem*)HasSwitch(lpszSwitch);
	if (p == NULL || p->m_aValues.IsEmpty())
		return lpszDefault ? lpszDefault : EMPTY_STR;
	return p->m_aValues[0];
}
