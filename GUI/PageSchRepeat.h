#if !defined(AFX_PAGESCHREPEAT_H__849D3294_4543_4902_9A16_73D4FD147CC0__INCLUDED_)
#define AFX_PAGESCHREPEAT_H__849D3294_4543_4902_9A16_73D4FD147CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSchRepeat.h : header file
//

#include "External\\EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSchRepeat dialog

class CPageSchRepeat : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSchRepeat)

// Construction
public:
	int GetRepeat() const;
	void SetData(int nRepeat);
	CPageSchRepeat();
	~CPageSchRepeat();

// Dialog Data
	//{{AFX_DATA(CPageSchRepeat)
	enum { IDD = IDD_PAGE_SCHREPEAT };
	CEditEx	m_wndRpt;
	int		m_nRepeat;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSchRepeat)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSchRepeat)
	afx_msg void OnType();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCHREPEAT_H__849D3294_4543_4902_9A16_73D4FD147CC0__INCLUDED_)
