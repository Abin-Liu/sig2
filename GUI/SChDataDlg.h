#if !defined(AFX_SCHDATADLG_H__1AD84CCF_EF55_465D_938F_C28701C29592__INCLUDED_)
#define AFX_SCHDATADLG_H__1AD84CCF_EF55_465D_938F_C28701C29592__INCLUDED_

#include "SIGDataDef.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SChDataDlg.h : header file
//

#include "External\\EditEx.h"
#include "SIGData.h"

/////////////////////////////////////////////////////////////////////////////
// SChDataDlg dialog

class SChDataDlg : public CDialog
{
// Construction
public:
	const CScheduleData* GetData() const;
	SChDataDlg(const CSIGData* psd, const CScheduleData* pData = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SChDataDlg)
	enum { IDD = IDD_SCHDLG };
	CEditEx	m_wndStopParam2;
	CEditEx	m_wndStopParam1;
	CEditEx	m_wndStartParam2;
	CEditEx	m_wndStartParam1;
	CComboBox	m_wndCombo;
	int		m_nStartRule;
	int		m_nStopRule;
	int		m_nStartParam1;
	int		m_nStartParam2;
	int		m_nStopParam1;
	int		m_nStopParam2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SChDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SChDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnConditions();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	static void CALLBACK EnumInstProc(const CSIGInstance* pInstance, LPARAM lParam);

	CScheduleData m_data;	
	const CSIGData* m_psd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHDATADLG_H__1AD84CCF_EF55_465D_938F_C28701C29592__INCLUDED_)
