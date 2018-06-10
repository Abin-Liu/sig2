#if !defined(AFX_ACCOUNTDLG_H__4DD20E20_9739_4286_8BAE_9DEF4C0F55B7__INCLUDED_)
#define AFX_ACCOUNTDLG_H__4DD20E20_9739_4286_8BAE_9DEF4C0F55B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountDlg.h : header file
//

#include "Mule.h"
#include "External\\RealmInfo.h"
#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog

class CAccountDlg : public CDialog
{
// Construction
public:
	const CMuleData& GetData() const;
	CAccountDlg(const CMuleData* pData, CRealmInfo* pri, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountDlg)
	enum { IDD = IDD_ACCOUNTDLG };
	CEditEx	m_wndPwd2;
	CEditEx	m_wndSubRealm;
	CEditEx	m_wndPwd1;
	CEditEx	m_wndBnetIP;
	CEditEx	m_wndAccount;
	CComboBox	m_wndRealms;
	CString	m_sAccount;
	CString	m_sBnetIP;
	int		m_nCharCount;
	CString	m_sPwd1;
	CString	m_sSubRealm;
	CString	m_sPwd2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeRealms();
	afx_msg void OnRealmedit();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	void ListRealms();
	CMuleData m_data;
	CRealmInfo* m_pri;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTDLG_H__4DD20E20_9739_4286_8BAE_9DEF4C0F55B7__INCLUDED_)
