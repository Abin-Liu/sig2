#if !defined(AFX_OPTPAGEMULE_H__A56BE848_5BD4_4B7A_91EA_B45EEE2E056E__INCLUDED_)
#define AFX_OPTPAGEMULE_H__A56BE848_5BD4_4B7A_91EA_B45EEE2E056E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPageMule.h : header file
//
#include "Mule.h"
#include "External\\EditEx.h"
#include "External\\ReportCtrl.h"
#include "External\\RealmInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CPageMule dialog

class CPageMule : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMule)

// Construction
public:
	const CMule* GetMule() const;
	void SetData(const CMule* pMule, CRealmInfo* pri);
	CPageMule();
	~CPageMule();

// Dialog Data
	//{{AFX_DATA(CPageMule)
	enum { IDD = IDD_PAGE_MULE };
	CEditEx	m_wndGameLen;
	CReportCtrl	m_wndList;
	int		m_nMuleLen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMule)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMule)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	CMule m_mule;
	CRealmInfo* m_pri;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGEMULE_H__A56BE848_5BD4_4B7A_91EA_B45EEE2E056E__INCLUDED_)
