#if !defined(AFX_OPTPAGELOGIN_H__EA01CB24_7780_4BE6_A4FD_DA70CDDCCF29__INCLUDED_)
#define AFX_OPTPAGELOGIN_H__EA01CB24_7780_4BE6_A4FD_DA70CDDCCF29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPageLogin.h : header file
//
#include "SIGDataDef.h"
#include "External\\RealmInfo.h"
#include "External\\EditEx.h"
#include "External\\RealmInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CPageLogin dialog

class CPageLogin : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageLogin)

// Construction
public:

	void SetData(CLoginData* pData, CRealmInfo* pri);
	CPageLogin();
	~CPageLogin();

// Dialog Data
	//{{AFX_DATA(CPageLogin)
	enum { IDD = IDD_PAGE_LOGIN };
	CEditEx	m_wndSubRealm;
	CEditEx	m_wndBnetIP;
	CComboBox	m_wndRealms;
	CEditEx	m_wndPwd2;
	CEditEx	m_wndPwd1;
	CEditEx	m_wndAccount;
	CString	m_sAccount;
	int		m_nCharPos;
	CString	m_sPwd1;
	CString	m_sPwd2;
	CString	m_sSubRealm;
	CString	m_sBnetIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageLogin)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnRealmedit();
	afx_msg void OnSelchangeRealms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void ListRealms();

	CRealmInfo* m_pri;
	CLoginData* m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGELOGIN_H__EA01CB24_7780_4BE6_A4FD_DA70CDDCCF29__INCLUDED_)
