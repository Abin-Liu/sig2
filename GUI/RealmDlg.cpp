// RealmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "RealmDlg.h"
#include "RealmEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealmDlg dialog


CRealmDlg::CRealmDlg(CRealmInfo* pri, CWnd* pParent /*=NULL*/)
	: CDialog(CRealmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pri = pri;
	ASSERT(m_pri);
}


void CRealmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealmDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealmDlg, CDialog)
	//{{AFX_MSG_MAP(CRealmDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealmDlg message handlers

void CRealmDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CRealmEditDlg dlg("", "", "", this);
	if (dlg.DoModal() != IDOK)
		return;

	int nRes = m_pri->AddRealm(dlg.m_sName, dlg.m_sServer, dlg.m_sSub);

	if (nRes == RI_ERR_OK)
	{
		m_wndList.SetFocus();
		const int n = m_wndList.InsertItem(INT_MAX, dlg.m_sName);
		m_wndList.SetItemText(n, 1, dlg.m_sServer);
		m_wndList.SetItemText(n, 2, dlg.m_sSub);
		m_wndList.SetItemStates(n, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
	}
	else if (nRes == RI_ERR_ALREADYEXIST)
	{
		CString sErr;
		sErr.Format(g_bLanguage ? "国度名\"%s\"已经存在。" : "Realm \"%s\" already exists.", dlg.m_sName);
		MessageBox(sErr, g_bLanguage ? "重复国度名" : "Duplicated Realm Name", MB_ICONEXCLAMATION);
	}
}

void CRealmDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CString str = m_wndList.GetItemText(IDX, 0);
	if (m_pri->IsOfficialRealm(str))
		return;

	CRealmEditDlg dlg(m_wndList.GetItemText(IDX, 0), m_wndList.GetItemText(IDX, 1), m_wndList.GetItemText(IDX, 2), this);
	if (dlg.DoModal() != IDOK)
		return;

	int nRes = m_pri->EditRealm(m_wndList.GetItemText(IDX, 0), dlg.m_sName, dlg.m_sServer, dlg.m_sSub);
	if (nRes == RI_ERR_OK)
	{
		m_wndList.SetItemText(IDX, 0, dlg.m_sName);
		m_wndList.SetItemText(IDX, 1, dlg.m_sServer);
		m_wndList.SetItemText(IDX, 2, dlg.m_sSub);
		m_wndList.SetFocus();
	}
	else if (nRes == RI_ERR_ALREADYEXIST)
	{
		CString sErr;
		sErr.Format(g_bLanguage ? "国度名\"%s\"已经存在。" : "Realm \"%s\" already exists.", dlg.m_sName);
		MessageBox(sErr, g_bLanguage ? "重复国度名" : "Duplicated Realm Name", MB_ICONEXCLAMATION);
	}
}

void CRealmDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CString str = m_wndList.GetItemText(IDX, 0);
	if (m_pri->IsOfficialRealm(str))
		return;

	if (m_pri->RemoveRealm(str) == RI_ERR_OK)
		m_wndList.DeleteItem(IDX);
}

void CRealmDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_pri->SavePrivateRealms(AfxGetAppEx()->GetAppDir() + REALM_FILE);
	CDialog::OnOK();
}

BOOL CRealmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.SetColumnHeader(g_bLanguage ? "显示名称, 120;服务器,120;子国度,90" : "Display Name, 120;Server,120;Sub-Realm,90");
	if (g_bLanguage)
	{
		SetWindowText("国度编辑");
		SetDlgItemText(IDOK, "确定");
		SetDlgItemText(IDCANCEL, "取消");
		SetDlgItemText(IDC_ADD, "添加");
		SetDlgItemText(IDC_EDIT, "编辑");
		SetDlgItemText(IDC_DELETE, "删除");
	}
	ListRealms();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealmDlg::ListRealms(int nOpt)
{
	int nIndex = 0;

	while (TRUE)
	{
		TCHAR szName[512] = _T("");
		TCHAR szBnetIP[512] = _T("");
		TCHAR szPreferred[512] = _T("");
		
		if (m_pri->GetRealmByIndex(nIndex++, szName, szBnetIP, szPreferred, nOpt))
		{
			int n = m_wndList.InsertItem(INT_MAX, szName);
			m_wndList.SetItemText(n, 1, szBnetIP);
			m_wndList.SetItemText(n, 2, szPreferred);
		}
		else
		{
			break;
		}
	}
}

void CRealmDlg::CheckCurrentRealm()
{
	TCHAR szBnetIP[512] = _T("");
	TCHAR szPreferred[512] = _T("");

	if (!m_pri->GetDefaultRealm(szBnetIP, szPreferred))
		return;

	TCHAR szName[512] = _T("");

	if (!m_pri->FindRealmName(szBnetIP, szPreferred, szName))
		return;

	m_wndList.SetAllItemStates(RC_ITEM_CHECKED, RC_ITEM_UNCHECKED);

	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		if (m_wndList.GetItemText(i, 0).CompareNoCase(szName) == 0)
		{
			m_wndList.SetCheck(i, TRUE);
			m_wndList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			return;
		}
	}
}

void CRealmDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	BOOL bOK = FALSE;
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX != -1)
	{
		CString str = m_wndList.GetItemText(IDX, 0);
		bOK = !m_pri->IsOfficialRealm(str);
	}

	GetDlgItem(IDC_EDIT)->EnableWindow(bOK);
	GetDlgItem(IDC_DELETE)->EnableWindow(bOK);
	*pResult = 0;
}
