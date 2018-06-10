#if !defined(AFX_OPTPAGE7_H__7E6D6AE5_D2A8_472E_ABD1_0E188067C408__INCLUDED_)
#define AFX_OPTPAGE7_H__7E6D6AE5_D2A8_472E_ABD1_0E188067C408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptPage7.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageFinish dialog

class CPageFinish : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageFinish)

// Construction
public:
	CPageFinish();
	~CPageFinish();

// Dialog Data
	//{{AFX_DATA(CPageFinish)
	enum { IDD = IDD_PAGE_FINISH };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageFinish)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageFinish)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTPAGE7_H__7E6D6AE5_D2A8_472E_ABD1_0E188067C408__INCLUDED_)
