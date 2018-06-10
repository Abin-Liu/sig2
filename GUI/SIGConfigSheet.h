#if !defined(AFX_SIGCONFIGSHEET_H__5F9C953C_37E8_477A_A619_1B13476E9902__INCLUDED_)
#define AFX_SIGCONFIGSHEET_H__5F9C953C_37E8_477A_A619_1B13476E9902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIGConfigSheet.h : header file
//

#include "External\\KCSideBannerWnd.h"
#include "SIGData.h"
#include "PageThreadType.h"
#include "PageName.h"
#include "PageD2Exe.h"
#include "PageGame.h"
#include "PageHack.h"
#include "PageD2H.h"
#include "PageJSP.h"
#include "PageFinish.h"
#include "PageLogin.h"
#include "PageMule.h"

/////////////////////////////////////////////////////////////////////////////
// CSIGConfigSheet

class CSIGConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSIGConfigSheet)

// Construction
public:
	CSIGConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSIGConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIGConfigSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void Initialize(BOOL bEdit, const CSIGData* pData, CRealmInfo* pri, const CSIGInstance* pInstance);
	const CMule* GetMule() const;
	CSIGThreadData* GetThreadData();

	virtual ~CSIGConfigSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSIGConfigSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnPageThreadType(WPARAM wParam, LPARAM lParam);
	LRESULT OnWizardButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnBannerText(WPARAM wParam, LPARAM lParam);
	LRESULT OnHackType(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:	

	CPageThreadType m_pageThreadType;
	CPageName m_pageName;
	CPageD2Exe m_pageD2Exe;
	CPageGame m_pageGame;
	CPageHack m_pageHack;
	CPageD2H m_pageD2H;
	CPageJSP m_pageJSP;
	CPageFinish m_pageFinish;
	CPageLogin m_pageLogin;
	CPageMule m_pageMule;
	CKCSideBannerWnd m_wndBanner;
	CArrayEx<CPropertyPage*, CPropertyPage*> m_aAdded;
	CRealmInfo* m_pri;
	CSIGThreadData m_data;
	const CSIGData* m_pData;
	const CSIGInstance* m_pInstance;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGCONFIGSHEET_H__5F9C953C_37E8_477A_A619_1B13476E9902__INCLUDED_)
