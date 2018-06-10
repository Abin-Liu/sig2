// SIGConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "SIGConfigSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIGConfigSheet

IMPLEMENT_DYNAMIC(CSIGConfigSheet, CPropertySheet)

CSIGConfigSheet::CSIGConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSIGConfigSheet::CSIGConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pData = NULL;
	m_pInstance = NULL;
	m_pri = NULL;
	SetWizardMode();
}

CSIGConfigSheet::~CSIGConfigSheet()
{
}

BEGIN_MESSAGE_MAP(CSIGConfigSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSIGConfigSheet)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PAGE_THREADTYPE, OnPageThreadType)
	ON_MESSAGE(WM_WIZARD_BUTTON, OnWizardButton)
	ON_MESSAGE(WM_BANNER_TEXT, OnBannerText)
	ON_MESSAGE(WM_HACK_TYPE, OnHackType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIGConfigSheet message handlers



BOOL CSIGConfigSheet::OnInitDialog() 
{	
	m_wndBanner.Attach(this, KCSB_ATTACH_TOP);

	CPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_wndBanner.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON1), KCSB_ICON_LEFT | KCSB_ICON_VCENTER);
	SetWindowText(g_bLanguage ? "线程设置向导" : "Thread Configuration Wizard");
	SetTimer(0, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CSIGConfigSheet::OnWizardButton(WPARAM wParam, LPARAM lParam)
{
	SetWizardButtons(lParam);
	return 0;
}

LRESULT CSIGConfigSheet::OnBannerText(WPARAM wParam, LPARAM lParam)
{	
	m_wndBanner.SetTitle((LPCSTR)wParam);
	m_wndBanner.SetCaption((LPCSTR)lParam);
	return 0;
}

LRESULT CSIGConfigSheet::OnHackType(WPARAM wParam, LPARAM lParam)
{
	for (int i = m_aAdded.GetSize() - 1; i >= 0; i--)
	{
		RemovePage(m_aAdded[i]);
		m_aAdded.RemoveAt(i);
	}
	
	if (lParam == 1)
	{
		AddPage(&m_pageD2H);
		m_aAdded.Add(&m_pageD2H);
	}
	else if (lParam == 2)
	{
		AddPage(&m_pageJSP);
		m_aAdded.Add(&m_pageJSP);
	}

	AddPage(&m_pageFinish);
	m_aAdded.Add(&m_pageFinish);
	return 0;
}

CSIGThreadData* CSIGConfigSheet::GetThreadData()
{
	return &m_data;
}

const CMule* CSIGConfigSheet::GetMule() const
{
	return m_pageMule.GetMule();
}

BOOL CSIGConfigSheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CPropertySheet::PreTranslateMessage(pMsg);
}

void CSIGConfigSheet::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		static CString str = "";
		GetWindowText(str);
		if (str.IsEmpty())
			SetWindowText(g_bLanguage ? "线程设置向导" : "Thread Configuration Wizard");
	}
	CPropertySheet::OnTimer(nIDEvent);
}

void CSIGConfigSheet::OnDestroy() 
{
	KillTimer(0);
	CPropertySheet::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CSIGConfigSheet::Initialize(BOOL bEdit, const CSIGData* pData, CRealmInfo* pri, const CSIGInstance* pInstance)
{
	m_pData = pData;
	m_pri = pri;
	m_pInstance = pInstance;

	ASSERT(m_pData);
	ASSERT(m_pri);
	if (bEdit)
	{
		ASSERT(pInstance);
		m_data = *pInstance->GetData();
	}
	else
	{
		m_data.bAllowModifyScript = TRUE;
		m_data.sParamLine = "-noplugin -res800 -ns -lq";
		m_data.sName = pData->GetUniqueName(TRUE, "New Thread", NULL);
	}	

	m_pageThreadType.SetData(pData);
	m_pageName.SetData(&m_data);
	m_pageD2Exe.SetData(&m_data);
	m_pageMule.SetData(pInstance ? pInstance->GetMule() : NULL, m_pri);
	m_pageGame.SetData(&m_data);
	m_pageHack.SetData(pData, &m_data);
	m_pageD2H.SetData(&m_data);
	m_pageJSP.SetData(&m_data);
	m_pageLogin.SetData(&m_data.login, m_pri);

	if (!bEdit)
	{
		AddPage(&m_pageThreadType);
	}
	else
	{
		// add pages
		AddPage(&m_pageName);
		AddPage(&m_pageD2Exe);	

		if (pInstance->GetMule() != NULL)
		{
			AddPage(&m_pageMule);
		}
		else
		{
			AddPage(&m_pageLogin);
			AddPage(&m_pageGame);
		}	
	
		AddPage(&m_pageHack);
	}
}

LRESULT CSIGConfigSheet::OnPageThreadType(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam >= 0 && wParam < 3);
	if (wParam == 2)
		ASSERT(lParam);

	m_pInstance = wParam == 2 ? (const CSIGInstance*)lParam : NULL;
	if (m_pInstance)
	{		
		m_data = *m_pInstance->GetData();
		m_data.sName = m_pData->GetUniqueName(TRUE, m_data.sName);
	}

	m_pageName.SetData(&m_data);
	m_pageD2Exe.SetData(&m_data);
	m_pageMule.SetData(m_pInstance ? m_pInstance->GetMule() : NULL, m_pri);
	m_pageGame.SetData(&m_data);
	m_pageHack.SetData(m_pData, &m_data);
	m_pageD2H.SetData(&m_data);
	m_pageJSP.SetData(&m_data);
	m_pageLogin.SetData(&m_data.login, m_pri);

	AddPage(&m_pageName);
	AddPage(&m_pageD2Exe);
	
	if (wParam == 0)
	{			
		AddPage(&m_pageLogin);
		AddPage(&m_pageGame);		
	}
	else if (wParam == 1)
	{
		// add pages
		AddPage(&m_pageMule);		
	}
	else if (wParam == 2)
	{
		if (m_pInstance->GetMule() == NULL)
		{
			AddPage(&m_pageLogin);
			AddPage(&m_pageGame);
		}
		else
		{
			m_pageMule.SetData(m_pInstance->GetMule(), m_pri);
			AddPage(&m_pageMule);
		}
	}

	AddPage(&m_pageHack);
	return 0;
}
