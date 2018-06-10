//////////////////////////////////////////////////////////////////////////
//    Encrypt.h
//
// Functions exported by this file are used for text encryption & decryption,
// with optional password protection.
//
// 4/08/2003 Abin
//////////////////////////////////////////////////////////////////////////

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include <windows.h>

// The max length of a string that can be encrypted, to guarantee the output
// string not to exceed 32767(32KB) chars (limited by Win9x).
#define MAX_ENCRYPT_LENGTH	16000

// Operation results
enum ERRORCODE { ERR_OK = 0,				// Operation completed successful
				 ERR_NULL_STRING,			// Invalid string source(null pointer)
				 ERR_LENGTH_EXCEED,			// Source string length exceeds the limit
				 ERR_INSUFFICIENT_BUFFER,	// Insufficient buffer size
				 ERR_INVALID_PASSWORD,		// Invalid password
				 ERR_INVALID_SRC,			// Invalid source string
				 ERR_MORE_CHARS,			// There are more characters to be decrypted
				 ERR_INVALID_CHECKSUM };	// Checksum examination failed

DWORD __declspec(dllexport) GetEncryptLength(LPCSTR lpSrc);
DWORD __declspec(dllexport) GetDecryptLength(LPCSTR lpSrc, LPCSTR lpPassword = NULL);
BOOL __declspec(dllexport) IsPasswordProtected(LPCSTR lpSrc);
int __declspec(dllexport) Encrypt(LPCSTR lpSrc, LPSTR lpBuffer, DWORD dwBufferSize, LPCSTR lpPassword = NULL);
int __declspec(dllexport) Decrypt(LPCSTR lpSrc, LPSTR lpBuffer, DWORD dwMaxBufferSize, LPCSTR lpPassword = NULL);

DWORD __declspec(dllexport) CalcCRC32(LPCVOID lpData, DWORD dwSize);
DWORD __declspec(dllexport) CalcMD5(LPCVOID lpData, DWORD dwSize, LPSTR lpszBuffer, DWORD dwBufferSize);


#endif // #ifndef __ENCRYPT_H__
