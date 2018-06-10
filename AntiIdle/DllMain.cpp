//////////////////////////////////////////////////////////////////////
// DllMain.cpp
// -------------------------------------------------------------------
// Default Dll entrypoint.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Offsets.h"

HWND g_hD2Wnd = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);
void ClientStart();
void ClientStop();

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		g_hD2Wnd = NULL;
		::EnumWindows(EnumWindowsProc, 0);

		if (g_hD2Wnd)
		{
			for (int i = 0; NEEDED_DLLS[i]; i++)
				::LoadLibrary(NEEDED_DLLS[i]);

			DefineOffsets();
			ClientStart();
		}

		break;

	case DLL_PROCESS_DETACH:

		if (g_hD2Wnd)
		{
			ClientStop();
			for (int i = 0; NEEDED_DLLS[i]; i++)
			{
				HMODULE hModule = ::GetModuleHandle(NEEDED_DLLS[i]);
				if (hModule)
					::FreeLibrary(hModule);
			}
		}

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
