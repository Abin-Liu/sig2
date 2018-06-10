#if !defined(AFX_PAGESCHNAME_H__8307426F_6BFE_495B_B63D_E610F5DB3F6A__INCLUDED_)
#define AFX_PAGESCHNAME_H__8307426F_6BFE_495B_B63D_E610F5DB3F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSchName.h : header file
//
#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSchName dialog

class CPageSchName : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSchName)

// Construction
public:
	void SetData(LPCSTR lpszName);
	CPageSchName();
	~CPageSchName();

// Dialog Data
	//{{AFX_DATA(CPageSchName)
	enum { IDD = IDD_PAGE_SCHNAME };
	CEditEx	m_wndName;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSchName)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSchName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCHNAME_H__8307426F_6BFE_495B_B63D_E610F5DB3F6A__INCLUDED_)
