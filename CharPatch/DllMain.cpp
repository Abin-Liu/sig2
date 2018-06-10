//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////
#include <windows.h>

HWND g_hD2Wnd = NULL;
HHOOK g_hHook = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		g_hD2Wnd = NULL;
		::EnumWindows(EnumWindowsProc, 0);

		if (g_hD2Wnd)
			g_hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, (HINSTANCE)hModule, ::GetWindowThreadProcessId(g_hD2Wnd, NULL));

		if (g_hHook)
			DeleteFile("c:\\1.txt");

		break;

	case DLL_PROCESS_DETACH:

		if (g_hHook)
			::UnhookWindowsHookEx(g_hHook);

		break;
    } 

    return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	char classname[20];

	GetClassName(hwnd, classname, sizeof(classname));

	if(!stricmp(classname, "Diablo II"))
	{
		DWORD pid, tid;
		tid = GetWindowThreadProcessId( hwnd, &pid );
		if( pid == GetCurrentProcessId() )
		{
			g_hD2Wnd = hwnd;
			return FALSE;	// Stop enum
		}
	}

	return TRUE;	// Continue enum
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PCWPSTRUCT p = (PCWPSTRUCT)lParam;
	if (p && p->message == WM_ACTIVATEAPP && p->wParam == 0)
	{
		DeleteFile("c:\\2.txt");
		return 1;
	}
	
	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
