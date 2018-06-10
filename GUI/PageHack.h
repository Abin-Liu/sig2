#if !defined(AFX_OPTPAGE4_H__F275BACE_AB14_456B_81A3_E0AC0FA74988__INCLUDED_)
#define AFX_OPTPAGE4_H__F275BACE_AB14_456B_81A3_E0AC0FA74988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage4.h : header file
//
#include "SIGData.h"
#include "External\\BrowseCtrl.h"
#include "External\\HyperLink.h"
#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPageHack dialog

class CPageHack : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageHack)

// Construction
public:
	void SetData(const CSIGData* psd, CSIGThreadData* pData);
	CPageHack();
	~CPageHack();

// Dialog Data
	//{{AFX_DATA(CPageHack)
	enum { IDD = IDD_PAGE_HACK };
	CComboBox	m_wndCombo;
	CEditEx	m_wndBanRetry;
	CEditEx	m_wndMin2Games;
	CHyperLink	m_wndJspUrl;
	CBrowseCtrl	m_wndJspPath;
	CHyperLink	m_wndHackitUrl;
	CBrowseCtrl	m_wndHackitPath;
	int		m_nHackType;
	BOOL	m_bAntiIdle;
	BOOL	m_bMin2Games;
	int		m_nBanRule;
	int		m_nMin2Games;
	int		m_nBanRetry;
	CString	m_sSwitchThread;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageHack)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageHack)
	virtual BOOL OnInitDialog();
	afx_msg void OnStatsChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void CheckStats();
	static void CALLBACK EnumInstProc(const CSIGInstance* pInstance, LPARAM lParam);

	CSIGThreadData* m_pData;
	const CSIGData* m_psd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE4_H__F275BACE_AB14_456B_81A3_E0AC0FA74988__INCLUDED_)
