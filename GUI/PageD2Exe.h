#if !defined(AFX_OPTPAGE2_H__E0F1AD9E_0790_4C6B_B5D5_1AE062E58F49__INCLUDED_)
#define AFX_OPTPAGE2_H__E0F1AD9E_0790_4C6B_B5D5_1AE062E58F49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage2.h : header file
//

#include "External\\BrowseCtrl.h"
#include "External\\EditEx.h"
#include "SIGDataDef.h"

/////////////////////////////////////////////////////////////////////////////
// CPageD2Exe dialog

class CPageD2Exe : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageD2Exe)

// Construction
public:
	void SetData(CSIGThreadData* pData);
	CPageD2Exe();
	~CPageD2Exe();

// Dialog Data
	//{{AFX_DATA(CPageD2Exe)
	enum { IDD = IDD_PAGE_D2EXE };
	CEditEx	m_wndEdit;
	CBrowseCtrl	m_wndBrowse;
	CString	m_sParams;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageD2Exe)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageD2Exe)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CSIGThreadData* m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE2_H__E0F1AD9E_0790_4C6B_B5D5_1AE062E58F49__INCLUDED_)
