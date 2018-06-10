/////////////////////////////////////////////////////////////////////
// I believe these are Mousepad's code
// Also some were from D2BS.
//////////////////////////////////////////////////////////////////////
#ifndef __D2EXPFUNCS
#define __D2EXPFUNCS

#include "definitions.h"

#define D2EXPFUNC(d1,v1,t1,t2) typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;

D2EXPFUNC(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2))
D2EXPFUNC(D2CLIENT, PrintGameStringAtBottomLeft, void __fastcall, (wchar_t* text, int arg2))
D2EXPFUNC(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type));
D2EXPFUNC(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void));
D2EXPFUNC(D2COMMON, GetUnitModifiedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex));	 		// #10519
D2EXPFUNC(D2NET, Send, void __stdcall, (DWORD dwFlags, LPBYTE lpBuf, DWORD len));

#endif