// Sig2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sig2.h"
#include "MainFrm.h"
#include "External\\ReportCtrl.h"
#include "External\\StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define APP_TITLE	"SIG II"
#define APP_VER		"4.10"

BOOL g_bLanguage = FALSE;

BOOL IsIgnored(LPCSTR lpsz, const LPCSTR* ppIgnore)
{
	if (lpsz == NULL || ppIgnore == NULL)
		return FALSE;

	for (int i = 0; ppIgnore[i]; i++)
	{
		if (::stricmp(lpsz, ppIgnore[i]) == 0)
			return TRUE;
	}

	return FALSE;
}

int FindFromStrArray(const CStringArray& arr, LPCSTR lpsz)
{
	for (int i = 0; i < arr.GetSize(); i++)
	{
		if (arr[i].CompareNoCase(lpsz) == 0)
			return i;
	}

	return -1;
}

CString GetAppTitle()
{
	CString str;

	if (g_bLanguage)
		str.Format("%s %s°æ", APP_TITLE, APP_VER);
	else
		str.Format("%s v%s", APP_TITLE, APP_VER);
	return str;
}

int SplitString(LPCSTR lpsz, LPCSTR lpszDelimitor, CStringArray& aArray)
{
	aArray.RemoveAll();
	if (lpsz == NULL || lpszDelimitor == NULL)
		return 0;

	CStringEx str(lpsz);
	BOOL b = str.FindToken(lpszDelimitor, TRUE);
	while (b)
	{
		CStringEx sSeg;
		b = str.FindNextToken(sSeg);
		sSeg.Trim();
		if (!sSeg.IsEmpty())
			aArray.Add(sSeg);
	}
	return aArray.GetSize();
}

CString GetDir(LPCSTR lpszPath)
{
	CString sPath(lpszPath);

	int n = sPath.ReverseFind('\\');
	if (n == -1)
		return sPath;

	return sPath.Left(n);
}

CString ListModules(LPCSTR lpszPath, LPCSTR lpszAddPath, LPCSTR lpszExt, const CStringArray& aModules, CWnd* pList, const LPCSTR* ppIgnore)
{
	CString sPath = GetDir(lpszPath);
	if (lpszAddPath[0])
	{
		sPath += '\\';
		sPath += lpszAddPath;
	}
	
	sPath += "\\*.";
	sPath += lpszExt;

	CReportCtrl* p = (CReportCtrl*)pList;
	if (lpszPath == NULL || lpszExt == NULL || p == NULL || !::IsWindow(p->m_hWnd))
		return sPath;

	p->DeleteAllItems();

	CFileFind ff;
	BOOL b = ff.FindFile(sPath);
	while (b)
	{
		b = ff.FindNextFile();
		CString sTitle = ff.GetFileTitle();
		if (!IsIgnored(sTitle, ppIgnore))
		{
			const int IDX = p->InsertItem(INT_MAX, sTitle);
			if (FindFromStrArray(aModules, sTitle) != -1)
				p->SetItemStates(IDX, RC_ITEM_CHECKED);		
		}
	}
	
	ff.Close();
	return sPath;
}

/////////////////////////////////////////////////////////////////////////////
// CSigApp

BEGIN_MESSAGE_MAP(CSigApp, CWinApp)
	//{{AFX_MSG_MAP(CSigApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_ABOUT1, OnAppAbout)
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSigApp construction

CSigApp::CSigApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSigApp object

CSigApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSigApp initialization

BOOL CSigApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	CreateSingleton(_T("{BAD86547-CCC7-4686-A744-E24AC575F6DD}"));
	if (!IsInstanceUnique())
	{
		RestorePrevWnd();
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	::srand(::GetTickCount());

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	free((void*)m_pszAppName);
	m_pszAppName=_tcsdup(APP_TITLE);
	SetRegistryKey(_T("Abin"));
	WriteProfileString("", "Version", APP_VER);
	WriteProfileString("", "Path", GetAppDir());
	g_bLanguage = AfxGetApp()->GetProfileInt("", "Language", -1);
	if (g_bLanguage != 0 && g_bLanguage != 1)
	{
		char szDesktop[1024] = "";
		::SHGetSpecialFolderPath(NULL, szDesktop, CSIDL_STARTUP, FALSE);
		CString str(szDesktop);
		int n = str.ReverseFind('\\');
		if (n != -1)
			str = str.Mid(n + 1);

		g_bLanguage = str.CompareNoCase("Æô¶¯") == 0;
	}

	::CreateDirectory(GetAppDir() + "\\Log", NULL);

	CFileFind ff;
	BOOL b = ff.FindFile(GetAppDir() + "\\Log\\*.txt");
	while (b)
	{
		b = ff.FindNextFile();
		::DeleteFile(ff.GetFilePath());
	}
	ff.Close();

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	SetPrevWnd(pFrame->m_hWnd);
	pFrame->CenterWindow();
	pFrame->OnFinishCreation();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSigApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSigApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSigApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_VER, GetAppTitle());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSigApp::OnHelpContent() 
{
	// TODO: Add your command handler code here
	WinHelp(0, 0);
}

int CSigApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxGetApp()->WriteProfileInt("", "Language", !!g_bLanguage);
	return CWinApp::ExitInstance();
}

void CSigApp::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: Add your specialized code here and/or call the base class
	CString sHelp = g_bLanguage ? "\\Chinese.chm" : "\\English.chm";
	::ShellExecute(NULL, _T("open"), _T("hh.exe"), m_sAppDir + sHelp, m_sAppDir, SW_SHOW);
	//CWinApp::WinHelp(dwData, nCmd);
}
