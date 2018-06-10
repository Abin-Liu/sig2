#if !defined(AFX_REALMEDITDLG_H__E83F8D74_EB2E_416A_B4BC_F06CC3B8CC63__INCLUDED_)
#define AFX_REALMEDITDLG_H__E83F8D74_EB2E_416A_B4BC_F06CC3B8CC63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealmEditDlg.h : header file
//

#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CRealmEditDlg dialog

class CRealmEditDlg : public CDialog
{
// Construction
public:
	CRealmEditDlg(LPCSTR lpszName, LPCSTR lpszServer, LPCSTR lpszSub, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRealmEditDlg)
	enum { IDD = IDD_REALMEDITDLG };
	CEditEx	m_wndSub;
	CEditEx	m_wndServer;
	CEditEx	m_wndName;
	CString	m_sName;
	CString	m_sServer;
	CString	m_sSub;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealmEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealmEditDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALMEDITDLG_H__E83F8D74_EB2E_416A_B4BC_F06CC3B8CC63__INCLUDED_)
