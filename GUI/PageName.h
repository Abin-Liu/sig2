#if !defined(AFX_OPTPAGE1_H__E56511AA_9CFB_4893_8E27_CDC133479CA8__INCLUDED_)
#define AFX_OPTPAGE1_H__E56511AA_9CFB_4893_8E27_CDC133479CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage1.h : header file
//

#include "External\\EditEx.h"
#include "SIGDataDef.h"

/////////////////////////////////////////////////////////////////////////////
// CPageName dialog

class CPageName : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageName)

// Construction
public:
	void SetData(CSIGThreadData* pData);
	CPageName();
	~CPageName();

// Dialog Data
	//{{AFX_DATA(CPageName)
	enum { IDD = IDD_PAGE_NAME };
	CEditEx	m_wndEdit;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageName)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CSIGThreadData* m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE1_H__E56511AA_9CFB_4893_8E27_CDC133479CA8__INCLUDED_)
