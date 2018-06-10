// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__23D9F23B_475A_4D03_B688_9BB45FF3D6F6__INCLUDED_)
#define AFX_STDAFX_H__23D9F23B_475A_4D03_B688_9BB45FF3D6F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define REALM_FILE		"\\RealmInfo.ini"
#define SIG_FILE		"\\sig.dat"
#define DICTIONARY_FILE	"\\Dictionary.txt"

enum {
		WM_TRAY_NOTIFY = WM_APP + 1,
		WM_PAGE_THREADTYPE,
		WM_PAGE_TEMPLATE,
		WM_WIZARD_BUTTON,
		WM_BANNER_TEXT,
		WM_CRE_TYPE,
		WM_HACK_TYPE,
		WM_INST_PLAYERHP,
		WM_INST_START,
		WM_INST_STOP,
		WM_INST_PAUSE,
		WM_INST_RESUME,
		WM_INST_STRING,
		WM_INST_ERROR,
		WM_INST_PLAYER,
		WM_SCH_START,
		WM_SCH_STOP,
		WM_SCH_PAUSE,
		WM_SCH_RESUME,
		WM_SCH_STRING,
		WM_SCH_ERROR,
		WM_THREAD_COUNT,
		WM_SCHEDULE_COUNT,
		WM_POPUP_MENU
	};

extern BOOL g_bLanguage;
extern HWND g_hViewWnd;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__23D9F23B_475A_4D03_B688_9BB45FF3D6F6__INCLUDED_)
