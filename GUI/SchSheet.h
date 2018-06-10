#if !defined(AFX_SCHSHEET_H__71FD6FA5_3FEB_44F6_9160_47F468CA0D83__INCLUDED_)
#define AFX_SCHSHEET_H__71FD6FA5_3FEB_44F6_9160_47F468CA0D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SchSheet.h : header file
//
#include "External\\KCSideBannerWnd.h"
#include "PageSchName.h"
#include "PageSchData.h"
#include "PageSchRepeat.h"
#include "PageFinish.h"
#include "SIGData.h"
#include "SIGDataDef.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CSchSheet

class CSchSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSchSheet)

// Construction
public:
	CSchSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSchSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSchSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetRepeat() const;
	const CScheduleData* GetData(int &rCount) const;
	CString GetName() const;
	void SetData(const CSIGData* psd, LPCSTR lpszSuggestedName, int nRepeat = 1, const CSIGSchedule* pSchedule = NULL);
	virtual ~CSchSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSchSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnWizardButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnBannerText(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:

	CKCSideBannerWnd m_wndBanner;
	CPageSchName m_pageName;
	CPageSchData m_pageData;
	CPageSchRepeat m_pageRepeat;
	CPageFinish m_pageFinish;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHSHEET_H__71FD6FA5_3FEB_44F6_9160_47F468CA0D83__INCLUDED_)
