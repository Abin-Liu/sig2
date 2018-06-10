#if !defined(AFX_GLOBALDLG_H__E5C8E97A_A0C0_45BF_ABBC_E3881B20B9BF__INCLUDED_)
#define AFX_GLOBALDLG_H__E5C8E97A_A0C0_45BF_ABBC_E3881B20B9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalDlg.h : header file
//

#include "SIGDataDef.h"

/////////////////////////////////////////////////////////////////////////////
// CGlobalDlg dialog

class CGlobalDlg : public CDialog
{
// Construction
public:
	LPCGLOBALDATA GetGlobal() const;
	CGlobalDlg(LPCGLOBALDATA pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGlobalDlg)
	enum { IDD = IDD_GLOBAL };
	CSpinButtonCtrl	m_wndSpin;
	int m_nMaxD2Process;
	int m_nLaunchDelay;
	int m_nConnectDelay;
	int m_nLoginDelay;
	int m_nCharSelDelay;
	int m_nCreateDelay;
	int m_nRestartDelay;
	int		m_nRnd1;
	int		m_nRnd2;
	int		m_nButtonDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGlobalDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void Var2Data();
	void Data2Var();

	GLOBALDATA m_data;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALDLG_H__E5C8E97A_A0C0_45BF_ABBC_E3881B20B9BF__INCLUDED_)
