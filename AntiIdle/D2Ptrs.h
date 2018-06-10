#ifndef __D2PTRS_H__
#define __D2PTRS_H__

#include <windows.h>
#include "definitions.h"

///////////////////////////////////////////////
// Originally Mousepad's & D2BS's Code
///////////////////////////////////////////////
enum {	DLLNO_D2CLIENT,
		DLLNO_D2COMMON,
		DLLNO_D2NET };

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))
#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);

#define D2FUNC_START D2CLIENT_PrintGameStringAtTopLeft
D2FUNCPTR(D2CLIENT, PrintGameStringAtTopLeft, void __fastcall, (wchar_t* text, int arg2), 0x7C600)
D2FUNCPTR(D2CLIENT, PrintGameStringAtBottomLeft, void __fastcall, (wchar_t* text, int arg2), 0x7C950)
D2FUNCPTR(D2CLIENT, GetUnit, UnitAny * __fastcall, (DWORD id, DWORD type), 0x869F0);
D2FUNCPTR(D2CLIENT, GetPlayerUnit, UnitPlayer * __fastcall, (void), 0x883D0);
D2FUNCPTR(D2COMMON, GetUnitModifiedStat, int __stdcall, (UnitAny * ptUnit, int StatId, int StatTblIndex), -0x2917);	 		// #10519
D2FUNCPTR(D2NET, Send, void __stdcall, (DWORD dwFlags, LPBYTE lpBuf, DWORD len), -0x2715);
#define D2FUNC_END D2NET_Send


#endif // __D2PTRS_H__