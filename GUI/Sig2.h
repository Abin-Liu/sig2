// Sig2.h : main header file for the SIG2 application
//

#if !defined(AFX_SIG2_H__29732490_7ADC_47FE_BA7B_EF86C95521FF__INCLUDED_)
#define AFX_SIG2_H__29732490_7ADC_47FE_BA7B_EF86C95521FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "External\\WinAppEx.h"
#include <AfxTempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSigApp:
// See Sig2.cpp for the implementation of this class
//

class CSigApp : public CWinAppEx
{
public:
	CSigApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CSigApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpContent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CString GetAppTitle();
CString GetDir(LPCSTR lpszPath);
int SplitString(LPCSTR lpsz, LPCSTR lpszDelimitor, CStringArray& aArray);
CString ListModules(LPCSTR lpszPath, LPCSTR lpszAddPath, LPCSTR lpszExt, const CStringArray& aModules, CWnd* pList, const LPCSTR* ppIgnore = NULL);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIG2_H__29732490_7ADC_47FE_BA7B_EF86C95521FF__INCLUDED_)
