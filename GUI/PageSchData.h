#if !defined(AFX_PAGESCHDATA_H__2E04472C_B08C_4D67_9E02_BB5C4665941F__INCLUDED_)
#define AFX_PAGESCHDATA_H__2E04472C_B08C_4D67_9E02_BB5C4665941F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSchData.h : header file
//

#include "SIGData.h"
#include "External\\ReportCtrl.h"
#include "SIGDataDef.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CPageSchData dialog

class CPageSchData : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSchData)

// Construction
public:
	const CScheduleData* GetData(int& rCount) const;
	CPageSchData();
	~CPageSchData();

	void SetData(const CSIGData* psd, const CSIGSchedule* pSchedule = NULL);

// Dialog Data
	//{{AFX_DATA(CPageSchData)
	enum { IDD = IDD_PAGE_SCHDATA };
	CReportCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSchData)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSchData)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static void MakeConditionStr(const CScheduleData* p, CString& rStart, CString& rStop);
	static void CALLBACK EnumTaskProc(const CScheduleData* pTask, LPARAM lParam);
	
	CArrayEx<CScheduleData, const CScheduleData&> m_aData;
	const CSIGSchedule* m_pSchedule;
	const CSIGData* m_psd; 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESCHDATA_H__2E04472C_B08C_4D67_9E02_BB5C4665941F__INCLUDED_)
