#if !defined(AFX_DICDLG_H__2FEAC037_3A2E_4D98_83AB_F941AEB0074A__INCLUDED_)
#define AFX_DICDLG_H__2FEAC037_3A2E_4D98_83AB_F941AEB0074A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DicDlg.h : header file
//

#include "NameDictionary.h"
#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDicDlg dialog

class CDicDlg : public CDialog
{
// Construction
public:
	const CStringArrayEx* GetDictionary() const;
	CDicDlg(const CStringArrayEx* pNames, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDicDlg)
	enum { IDD = IDD_DLGDLG };
	CEditEx	m_wndEdit;
	CString	m_sEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDicDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL IsDupedString(const CString& str) const;
	void SaveNames();

	CStringArrayEx m_aNames;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICDLG_H__2FEAC037_3A2E_4D98_83AB_F941AEB0074A__INCLUDED_)
