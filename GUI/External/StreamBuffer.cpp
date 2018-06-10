//////////////////////////////////////////////////////////////////////
// StreamBuffer.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StreamBuffer.h"
#include <assert.h>

CStreamBuffer::CStreamBuffer()
{
	m_pData = NULL;
	m_dwDataLen = 0;
	m_dwAllocLen = 0;
	m_dwStreamPos = 0;
}

CStreamBuffer::CStreamBuffer(const CStreamBuffer& rSrc)
{
	m_pData = NULL;
	m_dwDataLen = 0;
	m_dwAllocLen = 0;
	m_dwStreamPos = 0;
	Copy(rSrc);
}

CStreamBuffer::CStreamBuffer(LPCVOID lpData, DWORD dwDataLen)
{
	m_pData = NULL;
	m_dwDataLen = 0;
	m_dwAllocLen = 0;
	m_dwStreamPos = 0;
	Copy(lpData, dwDataLen);
}

CStreamBuffer::~CStreamBuffer()
{
	FreeData();
}

DWORD CStreamBuffer::Copy(const CStreamBuffer& rSrc)
{
	if (this == &rSrc)
		return m_dwDataLen;

	return Copy(rSrc.m_pData, rSrc.m_dwDataLen);
}

DWORD CStreamBuffer::Copy(LPCVOID lpData, DWORD dwDataLen)
{
	if (lpData == NULL || dwDataLen == 0)
	{
		FreeData();
		return 0;
	}

	if (!Inflate(dwDataLen))
		return 0;

	::memcpy(m_pData, lpData, dwDataLen);
	m_dwDataLen = dwDataLen;
	return m_dwDataLen;
}

const CStreamBuffer& CStreamBuffer::operator=(const CStreamBuffer& rSrc)
{
	Copy(rSrc);
	return *this;
}

BOOL CStreamBuffer::Inflate(DWORD dwNewSize)
{
	if (dwNewSize <= m_dwAllocLen)
		return TRUE;

	LPBYTE p = new BYTE[dwNewSize];
	if (p == NULL)
	{
		assert(FALSE);
		return FALSE;
	}

	if (m_pData && m_dwDataLen)
	{
		::memcpy(p, m_pData, m_dwDataLen);
		delete [] m_pData;
	}

	m_pData = p;
	m_dwAllocLen = dwNewSize;
	return TRUE;
}

void CStreamBuffer::FreeData()
{
	if (m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	m_dwDataLen = 0;
	m_dwAllocLen = 0;
}

DWORD CStreamBuffer::Append(LPCVOID lpData, DWORD dwDataLen)
{
	if (lpData == NULL || dwDataLen == 0)
		return 0;

	DWORD dwNewDataLen = m_dwDataLen + dwDataLen;
	DWORD dwNewAllocSize = m_dwAllocLen ? m_dwAllocLen : dwNewDataLen;
	while (dwNewAllocSize < dwNewDataLen)
		dwNewAllocSize *= 2;

	if (!Inflate(dwNewAllocSize))
		return 0;

	::memcpy(m_pData + m_dwDataLen, lpData, dwDataLen);
	m_dwDataLen = dwNewDataLen;
	return dwDataLen;
}

LPCVOID CStreamBuffer::GetDataPtr(DWORD dwOffset) const
{
	if (GetLength(dwOffset) == 0)
		return NULL;

	return (LPCVOID)((DWORD)m_pData + dwOffset);
}

DWORD CStreamBuffer::GetData(LPVOID lpBuffer, DWORD dwLength, DWORD dwOffset) const
{
	if (lpBuffer == NULL || dwLength == 0 || m_pData == NULL || m_dwDataLen == 0 || dwOffset >= m_dwDataLen)
		return 0;

	dwLength = min(m_dwDataLen - dwOffset, dwLength);
	if (dwLength == 0)
		return 0;

	::memcpy(lpBuffer, m_pData + dwOffset, dwLength);
	return dwLength;
}

DWORD CStreamBuffer::GetLength(DWORD dwOffset) const
{
	if (dwOffset >= m_dwDataLen)
		return 0;

	return m_dwDataLen - dwOffset;
}

void CStreamBuffer::FreeExtra()
{
	if (m_pData == NULL || m_dwDataLen >= m_dwAllocLen)
		return;

	if (m_dwDataLen == 0)
	{
		FreeData();
		return;
	}

	LPBYTE p = new BYTE[m_dwDataLen];
	if (p == NULL)
	{
		assert(FALSE);
		return;
	}

	::memcpy(p, m_pData, m_dwDataLen);
	delete [] m_pData;
	m_dwAllocLen = m_dwDataLen;
}

void CStreamBuffer::Clear(BOOL bFreeMemory)
{
	m_dwDataLen = 0;
	if (bFreeMemory)
		FreeData();
}

DWORD CStreamBuffer::ReadFromFile(LPCTSTR lpszFilePath)
{
	Clear(FALSE);
	if (lpszFilePath == NULL || lpszFilePath[0] == 0)
		return 0;

	HANDLE hFile = ::CreateFile(lpszFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwDummy;
	DWORD dwFileLen = ::GetFileSize(hFile, &dwDummy);
	if (dwFileLen > 0 && Inflate(dwFileLen))
	{
		dwDummy = 0;
		BOOL bOK = ::ReadFile(hFile, m_pData, dwFileLen, &dwDummy, NULL);
		if (!bOK)
			dwDummy = 0;
	}

	::CloseHandle(hFile);
	m_dwDataLen = dwDummy;
	return m_dwDataLen;
}

DWORD CStreamBuffer::WriteToFile(LPCTSTR lpszFilePath) const
{
	if (lpszFilePath == NULL || lpszFilePath[0] == 0)
		return 0;

	HANDLE hFile = ::CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwWritten = 0;
	if (m_pData && m_dwDataLen)
	{
		BOOL bOK = ::WriteFile(hFile, m_pData, m_dwDataLen, &dwWritten, NULL);
		if (!bOK)
			dwWritten = 0;
	}

	::CloseHandle(hFile);
	return dwWritten;
}

DWORD CStreamBuffer::SetPosition(DWORD dwNewPos)
{
	DWORD dwOld = m_dwStreamPos;
	m_dwStreamPos = dwNewPos;
	return dwOld;
}

DWORD CStreamBuffer::GetPosition() const
{
	return m_dwStreamPos;
}

BOOL CStreamBuffer::IsEof() const
{
	return m_dwStreamPos >= m_dwDataLen;
}

DWORD CStreamBuffer::StreamRead(LPVOID lpBuffer, DWORD dwLength)
{
	DWORD dwRead = GetData(lpBuffer, dwLength, m_dwStreamPos);
	m_dwStreamPos += dwRead;
	return dwRead;
}

DWORD CStreamBuffer::Overwrite(LPCVOID lpData, DWORD dwLength, DWORD dwOffset)
{
	if (lpData == NULL || dwLength == 0)
		return 0;

	DWORD dwLimit = GetLength(dwOffset);
	dwLength = min(dwLength, dwLimit);
	if (dwLength)
		::memcpy(m_pData + dwOffset, lpData, dwLength);
	return dwLength;
}

CStreamBuffer& CStreamBuffer::operator << (char val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (BYTE val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (short val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (WORD val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (int val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (UINT val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (long val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (DWORD val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (float val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (double val)
{
	Append(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator << (LPCTSTR val)
{
	ASSERT(val);
	const int LEN = _tcslen(val) + 1; // including null terminator
	Append(&LEN, sizeof(LEN));
	Append(val, LEN * sizeof(TCHAR));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (char& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (BYTE& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (short& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (WORD& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (int& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (UINT& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (long& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (DWORD& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (float& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (double& val)
{
	StreamRead(&val, sizeof(val));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (LPTSTR val)
{
	ASSERT(val);
	val[0] = 0;
	int nLen = 0;
	if (StreamRead(&nLen, sizeof(nLen)) == sizeof(nLen))
		StreamRead(val, nLen * sizeof(TCHAR)); // Dangerous!
	return *this;
}

#ifdef __AFXWIN_H__
CStreamBuffer& CStreamBuffer::operator << (const CString& val)
{
	const int LEN = val.GetLength() + 1; // including null terminator
	LPCTSTR psz = LEN > 0 ? (LPCTSTR)val : _T("");
	Append(&LEN, sizeof(LEN));
	Append(psz, LEN * sizeof(TCHAR));
	return *this;
}

CStreamBuffer& CStreamBuffer::operator >> (CString& val)
{
	val.Empty();
	int nLen = 0;
	if (StreamRead(&nLen, sizeof(nLen)) == sizeof(nLen) && nLen < 65535)
	{
		LPTSTR psz = new TCHAR[nLen];
		if (psz == NULL)
		{
			ASSERT(FALSE);
		}
		else
		{
			StreamRead(psz, nLen * sizeof(TCHAR));
			val = psz;
			delete [] psz;
		}
	}

	return *this;
}
#endif // __AFXWIN_H__


