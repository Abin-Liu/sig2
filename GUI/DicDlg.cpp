// DicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sig2.h"
#include "DicDlg.h"
#include "External\\StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDicDlg dialog


CDicDlg::CDicDlg(const CStringArrayEx* pNames, CWnd* pParent /*=NULL*/)
	: CDialog(CDicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDicDlg)
	m_sEdit = _T("");
	//}}AFX_DATA_INIT

	if (pNames)
		m_aNames.Copy(*pNames);

	for (int i = 0; i < m_aNames.GetSize(); i++)
		m_sEdit += m_aNames[i] + "\r\n";
}


void CDicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDicDlg)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Text(pDX, IDC_EDIT1, m_sEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDicDlg, CDialog)
	//{{AFX_MSG_MAP(CDicDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDicDlg message handlers

void CDicDlg::SaveNames()
{
	m_aNames.RemoveAll();
	CStringEx str(m_sEdit);
	BOOL b = str.FindToken("\r\n", TRUE);
	while (b)
	{
		CStringEx s;
		b = str.FindNextToken(s);
		s.Trim();
		if (!s.IsEmpty())
		{
			CString sAdd = s;
			if (!IsDupedString(sAdd))
				m_aNames.Add(sAdd);
		}
	}
}

BOOL CDicDlg::IsDupedString(const CString &str) const
{
	for (int i = 0; i < m_aNames.GetSize(); i++)
	{
		if (str.CompareNoCase(m_aNames[i]) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CDicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (g_bLanguage)
	{
		SetWindowText("词典编辑");
		SetDlgItemText(IDOK, "确定");
		SetDlgItemText(IDCANCEL, "取消");
		SetDlgItemText(IDC_LBL_EDIT, "每个游戏名一行，建议提供10行以上各不相同的游戏名:");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDicDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_sEdit.TrimLeft();
	m_sEdit.TrimRight();
	SaveNames();

	if (m_aNames.GetSize() < 10)
	{
		CString sMsg;
		if (g_bLanguage)
			sMsg.Format("你仅仅提供了%d行各不相同的游戏名，这样很可能会经常因为\"游戏已存在\"而导致创建游戏失败。\n\n确定要保存词典吗？", m_aNames.GetSize());
		else
			sMsg.Format("You only provided %d distinct names, this might cause game creation failures due to the \"Game Already Exists\" error.\n\nAre you sure you want to save the dictionary?", m_aNames.GetSize());

		if (MessageBox(sMsg, "Dictionary", MB_YESNO) != IDYES)
		{
			m_wndEdit.SetFocus();
			m_wndEdit.Highlight();
			return;
		}
	}
	EndDialog(IDOK);
}

const CStringArrayEx* CDicDlg::GetDictionary() const
{
	return &m_aNames;
}
