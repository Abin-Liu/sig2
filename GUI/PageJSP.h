#if !defined(AFX_OPTPAGE6_H__55BC8B21_93D1_4458_8D00_F179C5A85C71__INCLUDED_)
#define AFX_OPTPAGE6_H__55BC8B21_93D1_4458_8D00_F179C5A85C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage6.h : header file
//
#include "SIGDataDef.h"
#include "External\\ReportCtrl.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CPageJSP dialog

class CPageJSP : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageJSP)

// Construction
public:
	void SetData(CSIGThreadData* pData);
	CPageJSP();
	~CPageJSP();

// Dialog Data
	//{{AFX_DATA(CPageJSP)
	enum { IDD = IDD_PAGE_JSP };
	CReportCtrl	m_wndList;
	BOOL	m_bAllowModify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageJSP)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageJSP)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	CSIGThreadData* m_pData;
	CStringArray m_aModules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE6_H__55BC8B21_93D1_4458_8D00_F179C5A85C71__INCLUDED_)
