// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C2E79007_1B5A_46E1_8024_B60F1E81F43E__INCLUDED_)
#define AFX_MAINFRM_H__C2E79007_1B5A_46E1_8024_B60F1E81F43E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SigView.h"
#include "External\\BCMenu.h"
#include "External\\SystemTray.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void OnFinishCreation();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndThreadBarEng;
	CToolBar    m_wndThreadBarChs;
	CToolBar    m_wndSchBarEng;
	CToolBar    m_wndSchBarChs;
	CSigView    m_wndView;
	HICON		m_hIcon;
	BCMenu		m_menuEng;
	BCMenu		m_menuChs;
	BCMenu		m_popupEng;
	BCMenu		m_popupChs;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnLangEng();
	afx_msg void OnUpdateLangEng(CCmdUI* pCmdUI);
	afx_msg void OnLangChs();
	afx_msg void OnUpdateLangChs(CCmdUI* pCmdUI);
	afx_msg void OnViewThreads();
	afx_msg void OnUpdateViewThreads(CCmdUI* pCmdUI);
	afx_msg void OnViewSchedules();
	afx_msg void OnUpdateViewSchedules(CCmdUI* pCmdUI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPopupShowsig();
	afx_msg void OnUpdatePopupShowsig(CCmdUI* pCmdUI);
	afx_msg void OnPopupHidesig();
	afx_msg void OnUpdatePopupHidesig(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePanes(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadCount(WPARAM wParam, LPARAM lParam);
	LRESULT OnScheduleCount(WPARAM wParam, LPARAM lParam);
	LRESULT OnPopup(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void UpdateToobars();
	BOOL CreateToolbar(CToolBar* pBar, UINT nResID);
	void OnChangeLanguage(BOOL bLanguage);
	CSystemTray m_tray;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C2E79007_1B5A_46E1_8024_B60F1E81F43E__INCLUDED_)
