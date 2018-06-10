#if !defined(AFX_STATSDLG_H__BDE3B3C4_0795_4D83_91AB_6BAFE94B2419__INCLUDED_)
#define AFX_STATSDLG_H__BDE3B3C4_0795_4D83_91AB_6BAFE94B2419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatsDlg.h : header file
//

#include "SIGDataDef.h"

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog

class CStatsDlg : public CDialog
{
// Construction
public:
	CStatsDlg(LPCSTR lpszThreadName, LPCSIGSTATS pStats, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatsDlg)
	enum { IDD = IDD_STATSDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	static LPCSTR GetTimeStr(time_t time);
	static LPCSTR GetLengthStr(int nLength, int nDivisor = 0);
	static LPCSTR GetNumStr(int nNum, int nDivisor = 0);
	void Update();
	
	LPCSIGSTATS m_pStats;
	CString m_sName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATSDLG_H__BDE3B3C4_0795_4D83_91AB_6BAFE94B2419__INCLUDED_)
