#if !defined(AFX_OPTPAGE5_H__1A925D89_7AA1_4985_8803_AA0C333BF977__INCLUDED_)
#define AFX_OPTPAGE5_H__1A925D89_7AA1_4985_8803_AA0C333BF977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage5.h : header file
//
#include "SIGDataDef.h"
#include "External\\ReportCtrl.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPageD2H dialog

class CPageD2H : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageD2H)

// Construction
public:
	void SetData(CSIGThreadData* pData);
	CPageD2H();
	~CPageD2H();

// Dialog Data
	//{{AFX_DATA(CPageD2H)
	enum { IDD = IDD_PAGE_D2H };
	CReportCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageD2H)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageD2H)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	CSIGThreadData* m_pData;
	CStringArray m_aModules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE5_H__1A925D89_7AA1_4985_8803_AA0C333BF977__INCLUDED_)
