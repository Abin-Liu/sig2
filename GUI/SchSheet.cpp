// SchSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "SchSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSchSheet

IMPLEMENT_DYNAMIC(CSchSheet, CPropertySheet)

CSchSheet::CSchSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSchSheet::CSchSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	SetWizardMode();
	AddPage(&m_pageName);
	AddPage(&m_pageData);
	AddPage(&m_pageRepeat);
	AddPage(&m_pageFinish);
}

CSchSheet::~CSchSheet()
{
}


BEGIN_MESSAGE_MAP(CSchSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSchSheet)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WIZARD_BUTTON, OnWizardButton)
	ON_MESSAGE(WM_BANNER_TEXT, OnBannerText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchSheet message handlers

BOOL CSchSheet::OnInitDialog() 
{
	m_wndBanner.Attach(this, KCSB_ATTACH_TOP);

	CPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_wndBanner.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON2), KCSB_ICON_LEFT | KCSB_ICON_VCENTER);
	SetWindowText(g_bLanguage ? "计划设置向导" : "Schedule Configuration Wizard");
	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CSchSheet::OnWizardButton(WPARAM wParam, LPARAM lParam)
{
	SetWizardButtons(lParam);
	return 0;
}

LRESULT CSchSheet::OnBannerText(WPARAM wParam, LPARAM lParam)
{
	m_wndBanner.SetTitle((LPCSTR)wParam);
	m_wndBanner.SetCaption((LPCSTR)lParam);
	return 0;
}

void CSchSheet::SetData(const CSIGData* psd, LPCSTR lpszSuggestedName, int nRepeat, const CSIGSchedule* pSchedule)
{
	m_pageRepeat.SetData(nRepeat);
	m_pageName.SetData(lpszSuggestedName);
	m_pageData.SetData(psd, pSchedule);
}

CString CSchSheet::GetName() const
{
	return m_pageName.m_sName;
}

const CScheduleData* CSchSheet::GetData(int &rCount) const
{
	return m_pageData.GetData(rCount);
}

void CSchSheet::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		static CString str = "";
		GetWindowText(str);
		if (str.IsEmpty())
			SetWindowText(g_bLanguage ? "计划设置向导" : "Schedule Configuration Wizard");
	}
	CPropertySheet::OnTimer(nIDEvent);
}

void CSchSheet::OnDestroy() 
{
	KillTimer(0);
	CPropertySheet::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

int CSchSheet::GetRepeat() const
{
	return m_pageRepeat.GetRepeat();
}
