// OptPageMule.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "PageMule.h"
#include "AccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMule property page

IMPLEMENT_DYNCREATE(CPageMule, CPropertyPage)

CPageMule::CPageMule() : CPropertyPage(CPageMule::IDD)
{
	//{{AFX_DATA_INIT(CPageMule)
	m_nMuleLen = 10;
	//}}AFX_DATA_INIT
}

CPageMule::~CPageMule()
{
}

void CPageMule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMule)
	DDX_Control(pDX, IDC_MULECHARSTAYLENGTH, m_wndGameLen);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_MULECHARSTAYLENGTH, m_nMuleLen);
	DDV_MinMaxInt(pDX, m_nMuleLen, 5, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMule, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMule)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMule message handlers

BOOL CPageMule::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_WIZARD_BUTTON, 0, PSWIZB_BACK | PSWIZB_NEXT);
	GetParent()->SendMessage(WM_BANNER_TEXT, (WPARAM)"Mule Mode",
		(LPARAM)(g_bLanguage ? "设置Mule Mode登录信息。" : "Specify mule-mode login information."));
	return CPropertyPage::OnSetActive();
}

LRESULT CPageMule::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!UpdateData(TRUE))
		return 1;

	if (m_wndList.GetItemCount() == 0)
	{
		MessageBox("You need to add at least one account!", "No Accounts Found", MB_ICONEXCLAMATION);
		return 1;
	}

	m_mule.SetGameLen(m_nMuleLen);
	CMuleArray* pArray = m_mule.GetMuleArray();
	ASSERT(pArray);
	pArray->RemoveAll();

	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		CMuleData* p = (CMuleData*)m_wndList.GetItemData(i);
		if (p)
			pArray->Add(*p);
	}
	
	return CPropertyPage::OnWizardNext();
}

void CPageMule::SetData(const CMule* pMule, CRealmInfo* pri)
{
	ASSERT(pri);
	m_pri = pri;

	if (pMule)
		m_mule.Copy(pMule);
	m_nMuleLen = m_mule.GetGameLen();
	if (m_nMuleLen < 5)
		m_nMuleLen = 5;
}

BOOL CPageMule::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn(0, g_bLanguage ? "账号" : "Account", LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, g_bLanguage ? "服务器" : "Game Server", LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(2, g_bLanguage ? "国度" : "Sub Realm", LVCFMT_LEFT, 70);
	m_wndList.InsertColumn(3, g_bLanguage ? "角色数量" : "Players", LVCFMT_RIGHT, 50);

	if (g_bLanguage)
	{
		SetDlgItemText(IDC_LBL_LIST, "Mule Mode账号列表");
		SetDlgItemText(IDC_LBL_GAMELEN, "角色停留时间(秒):");
		SetDlgItemText(IDC_ADD, "添加");
		SetDlgItemText(IDC_EDIT, "编辑");
		SetDlgItemText(IDC_REMOVE, "删除");
		SetDlgItemText(IDC_MOVEUP, "上移");
		SetDlgItemText(IDC_MOVEDOWN, "下移");
	}
	
	CMuleArray* pArray = m_mule.GetMuleArray();

	for (int i = 0; i < pArray->GetSize(); i++)
	{
		CMuleData* p = new CMuleData;
		*p = pArray->GetAt(i);

		const int IDX = m_wndList.InsertItem(INT_MAX, p->login.sAccount);
		m_wndList.SetItemText(IDX, 1, p->login.sBnetIP);
		m_wndList.SetItemText(IDX, 2, p->login.sSubRealm);
		m_wndList.SetItemText(IDX, 3, p->nCount);
		m_wndList.SetItemData(IDX, (DWORD)p);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMule::OnDestroy() 
{
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		CMuleData* p = (CMuleData*)m_wndList.GetItemData(i);
		if (p)
			delete p;
		m_wndList.SetItemData(i, 0);
	}
	// TODO: Add your message handler code here
	CPropertyPage::OnDestroy();
}

const CMule* CPageMule::GetMule() const
{
	return m_mule.Verify() ? &m_mule : NULL;
}

void CPageMule::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CPageMule::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int COUNT = m_wndList.GetSelectedCount();
	GetDlgItem(IDC_REMOVE)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_EDIT)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_MOVEUP)->EnableWindow(COUNT == 1);
	GetDlgItem(IDC_MOVEDOWN)->EnableWindow(COUNT == 1);
	*pResult = 0;
}

void CPageMule::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CAccountDlg dlg(NULL, m_pri, this);
	if (dlg.DoModal() != IDOK)
		return;

	CMuleData* p = new CMuleData;
	*p = dlg.GetData();
	const int IDX = m_wndList.InsertItem(INT_MAX, p->login.sAccount);
	m_wndList.SetItemText(IDX, 1, p->login.sBnetIP);
	m_wndList.SetItemText(IDX, 2, p->login.sSubRealm);
	m_wndList.SetItemText(IDX, 3, p->nCount);
	m_wndList.SetItemData(IDX, (DWORD)p);
}

void CPageMule::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CMuleData* p = (CMuleData*)m_wndList.GetItemData(IDX);
	if (p == NULL)
		return;

	CAccountDlg dlg(p, m_pri, this);
	if (dlg.DoModal() != IDOK)
		return;
	
	*p = dlg.GetData();
	m_wndList.SetItemText(IDX, 0, p->login.sAccount);
	m_wndList.SetItemText(IDX, 1, p->login.sBnetIP);
	m_wndList.SetItemText(IDX, 2, p->login.sSubRealm);
	m_wndList.SetItemText(IDX, 3, p->nCount);
}

void CPageMule::OnRemove() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	CMuleData* p = (CMuleData*)m_wndList.GetItemData(IDX);
	if (p)
		delete p;
	m_wndList.SetItemData(IDX, 0);
	m_wndList.DeleteItem(IDX, TRUE);
}

void CPageMule::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_wndList.MoveUp(IDX);
}

void CPageMule::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX == -1)
		return;

	m_wndList.MoveDown(IDX);
}
