// SigView.h : interface of the CSigView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGVIEW_H__A3ED60AA_604D_4790_8715_5B91EE15DF7C__INCLUDED_)
#define AFX_SIGVIEW_H__A3ED60AA_604D_4790_8715_5B91EE15DF7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSigView window

#include "External\\ReportCtrl.h"
#include "SIGData.h"

class CSigView : public CWnd
{
// Construction
public:
	CSigView();

// Attributes
public:

// Operations
public:	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnFinishCreation();
	void HideAllD2Wnds(BOOL bHide) const;
	int GetViewType() const;
	void OnSwitchViewType(int nType);
	void OnChangeLanguage();
	virtual ~CSigView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSigView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKill();
	afx_msg void OnUpdateKill(CCmdUI* pCmdUI);
	afx_msg void OnPause();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnShowd2wnd();
	afx_msg void OnStart();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewlog(CCmdUI* pCmdUI);
	afx_msg void OnNew();
	afx_msg void OnConfig();
	afx_msg void OnUpdateConfig(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnGlobal();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateNew(CCmdUI* pCmdUI);
	afx_msg void OnSchNew();
	afx_msg void OnUpdateSchNew(CCmdUI* pCmdUI);
	afx_msg void OnDblclkThreadList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDblclkScheduleList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSchConfig();
	afx_msg void OnUpdateSchConfig(CCmdUI* pCmdUI);
	afx_msg void OnSchStart();
	afx_msg void OnUpdateSchStart(CCmdUI* pCmdUI);
	afx_msg void OnSchStop();
	afx_msg void OnUpdateSchStop(CCmdUI* pCmdUI);
	afx_msg void OnSchDelete();
	afx_msg void OnUpdateSchDelete(CCmdUI* pCmdUI);
	afx_msg void OnHidewnd();
	afx_msg void OnOptDictionary();
	afx_msg void OnThreadLog();
	afx_msg void OnThreadStatistics();
	afx_msg void OnUpdateThreadStatistics(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThreadLog(CCmdUI* pCmdUI);
	afx_msg void OnCusLog1();
	afx_msg void OnUpdateCusLog1(CCmdUI* pCmdUI);
	afx_msg void OnCusLog2();
	afx_msg void OnUpdateCusLog2(CCmdUI* pCmdUI);
	afx_msg void OnCusLog3();
	afx_msg void OnUpdateCusLog3(CCmdUI* pCmdUI);
	afx_msg void OnCusLog4();
	afx_msg void OnUpdateCusLog4(CCmdUI* pCmdUI);
	afx_msg void OnSelCustlog();
	afx_msg void OnViewlog();
	afx_msg void OnLeavegame();
	afx_msg void OnUpdateLeavegame(CCmdUI* pCmdUI);
	afx_msg void OnRclickThreadList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickScheduleList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT OnInstStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstPause(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstResume(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstString(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstError(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstPlayer(WPARAM wParam, LPARAM lParam);
	LRESULT OnInstPlayerHP(WPARAM wParam, LPARAM lParam);
	LRESULT OnScheduleStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnScheduleStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnScheduleString(WPARAM wParam, LPARAM lParam);
	LRESULT OnScheduleError(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	static void CALLBACK EnumSchProc(const CSIGSchedule* pSchedule, LPARAM lParam);
	static void CALLBACK EnumInstProc(const CSIGInstance* pInstance, LPARAM lParam);
	static void ListThread(const CSIGInstance* pInstance, CReportCtrl *pList, int nEditIndex = -1);
	static void ListSchedule(const CSIGSchedule *pSchedule, CReportCtrl *pList, int nEditIndex = -1);
	CSIGInstance* GetSelectedInstance(int* pIdx = NULL) const;
	CSIGSchedule* GetSelectedSchedule(int* pIdx = NULL) const;

	CReportCtrl m_wndThreads;
	CReportCtrl m_wndSchedules;
	CSIGData m_data;
	int m_nViewType;	
private:
	void ClickCustLog(int nIndex);
	void UpdateCustLogUI(CCmdUI* pCmdUI, int nIndex);
	void OpenLog(LPCSTR lpszLog) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGVIEW_H__A3ED60AA_604D_4790_8715_5B91EE15DF7C__INCLUDED_)
