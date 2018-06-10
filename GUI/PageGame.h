#if !defined(AFX_OPTPAGE3_H__87CFC6F8_1128_48DB_AF52_BD77519B2829__INCLUDED_)
#define AFX_OPTPAGE3_H__87CFC6F8_1128_48DB_AF52_BD77519B2829__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage3.h : header file
//
#include "SIGDataDef.h"
#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPageGame dialog

class CPageGame : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGame)

// Construction
public:
	void SetData(CSIGThreadData* pData);
	CPageGame();
	~CPageGame();

// Dialog Data
	//{{AFX_DATA(CPageGame)
	enum { IDD = IDD_PAGE_GAME };
	CEditEx	m_wndPrefix;
	CEditEx	m_wndFixedPwd;
	CEditEx	m_wndMaxInGame;
	CEditEx	m_wndChannel;
	CString	m_sChannel;
	int		m_nMaxInGame;
	int		m_nDifficulty;
	CString	m_sFixedPwd;
	int		m_nNameRule;
	CString	m_sPrefix;
	int		m_nFixedPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGame)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGame)
	virtual BOOL OnInitDialog();
	afx_msg void OnNametype();
	afx_msg void OnPwdType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void CheckStats();

	CSIGThreadData* m_pData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE3_H__87CFC6F8_1128_48DB_AF52_BD77519B2829__INCLUDED_)
