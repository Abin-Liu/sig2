//////////////////////////////////////////////////////////////////////
// StreamBuffer.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __STREAMBUFFER_H__
#define __STREAMBUFFER_H__

#include <windows.h>

class CStreamBuffer  
{
public:

	CStreamBuffer();
	CStreamBuffer(const CStreamBuffer& rSrc);
	CStreamBuffer(LPCVOID lpData, DWORD dwDataLen);
	virtual ~CStreamBuffer();

	DWORD GetLength(DWORD dwOffset = 0) const;
	LPCVOID GetDataPtr(DWORD dwOffset = 0) const;
	DWORD GetData(LPVOID lpBuffer, DWORD dwLength, DWORD dwOffset = 0) const;

	DWORD SetPosition(DWORD dwNewPos);
	DWORD GetPosition() const;
	BOOL IsEof() const;
	DWORD StreamRead(LPVOID lpBuffer, DWORD dwLength);
	DWORD Append(LPCVOID lpData, DWORD dwDataLen);

	DWORD Copy(const CStreamBuffer& rSrc);
	DWORD Copy(LPCVOID lpData, DWORD dwDataLen);	
	DWORD Overwrite(LPCVOID lpData, DWORD dwLength, DWORD dwOffset);
	void Clear(BOOL bFreeMemory = TRUE);
	void FreeExtra();

	DWORD ReadFromFile(LPCTSTR lpszFilePath);
	DWORD WriteToFile(LPCTSTR lpszFilePath) const;

	const CStreamBuffer& operator=(const CStreamBuffer& rSrc);

	CStreamBuffer& operator << (char val);
	CStreamBuffer& operator << (BYTE val);
	CStreamBuffer& operator << (short val);
	CStreamBuffer& operator << (WORD val);
	CStreamBuffer& operator << (int val);
	CStreamBuffer& operator << (UINT val);
	CStreamBuffer& operator << (long val);
	CStreamBuffer& operator << (DWORD val);
	CStreamBuffer& operator << (float val);
	CStreamBuffer& operator << (double val);
	CStreamBuffer& operator << (LPCTSTR val);

	CStreamBuffer& operator >> (char& val);
	CStreamBuffer& operator >> (BYTE& val);
	CStreamBuffer& operator >> (short& val);
	CStreamBuffer& operator >> (WORD& val);
	CStreamBuffer& operator >> (int& val);
	CStreamBuffer& operator >> (UINT& val);
	CStreamBuffer& operator >> (long& val);
	CStreamBuffer& operator >> (DWORD& val);
	CStreamBuffer& operator >> (float& val);
	CStreamBuffer& operator >> (double& val);
	CStreamBuffer& operator >> (LPTSTR val); // This is dangerous! Call at your own risk!

#ifdef __AFXWIN_H__
	CStreamBuffer& operator << (const CString& val);
	CStreamBuffer& operator >> (CString& val);
#endif // __AFXWIN_H__

protected:
	
	void FreeData();
	BOOL Inflate(DWORD dwNewSize);

	LPBYTE m_pData;
	DWORD m_dwDataLen;
	DWORD m_dwAllocLen;
	DWORD m_dwStreamPos;
};

#endif // __STREAMBUFFER_H__
