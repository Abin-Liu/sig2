#ifndef __OFFSETS_H__
#define __OFFSETS_H__

#include <windows.h>

const static LPCSTR NEEDED_DLLS[] = {"D2CLIENT.DLL",
									"D2COMMON.DLL",
									"D2NET.DLL",
									NULL };

DWORD GetDllOffset(LPCSTR dll, int offset);
DWORD GetDllOffset(int num);
void DefineOffsets();

#endif // __OFFSETS_H__