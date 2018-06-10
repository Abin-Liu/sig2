#if !defined(AFX_PAGETHREADTYPE_H__674CC857_1B73_477F_96A8_686E123FAD83__INCLUDED_)
#define AFX_PAGETHREADTYPE_H__674CC857_1B73_477F_96A8_686E123FAD83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageThreadType.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageThreadType dialog
#include "External\\ReportCtrl.h"
#include "SIGData.h"

class CPageThreadType : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageThreadType)

// Construction
public:
	void SetData(const CSIGData *pData);
	CPageThreadType();
	~CPageThreadType();

// Dialog Data
	//{{AFX_DATA(CPageThreadType)
	enum { IDD = IDD_PAGE_THREADTYPE };
	CReportCtrl	m_wndList;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageThreadType)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageThreadType)
	virtual BOOL OnInitDialog();
	afx_msg void OnType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static void CALLBACK EnumThreadProc(const CSIGInstance* pInstance, LPARAM lParam);

	const CSIGData* m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETHREADTYPE_H__674CC857_1B73_477F_96A8_686E123FAD83__INCLUDED_)
