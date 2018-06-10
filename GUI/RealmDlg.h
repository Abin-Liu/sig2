#if !defined(AFX_REALMDLG_H__B64ADD2B_5BF8_4255_B9EB_8891BCCCE3E6__INCLUDED_)
#define AFX_REALMDLG_H__B64ADD2B_5BF8_4255_B9EB_8891BCCCE3E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealmDlg.h : header file
//

#include "External\\ReportCtrl.h"
#include "External\\RealmInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CRealmDlg dialog

class CRealmDlg : public CDialog
{
// Construction
public:
	CRealmDlg(CRealmInfo* pri, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRealmDlg)
	enum { IDD = IDD_REALMDLG };
	CReportCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealmDlg)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckCurrentRealm();
	void ListRealms(int nOpt = RI_LIST_ALL);
	CRealmInfo* m_pri;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALMDLG_H__B64ADD2B_5BF8_4255_B9EB_8891BCCCE3E6__INCLUDED_)
