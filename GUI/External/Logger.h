//////////////////////////////////////////////////////////////////////
// Logger.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __LOGGER_H__
#define __LOGGER_H__

class CLogger  
{
public:
	
	CLogger();
	virtual ~CLogger();

	void AddLineBreak();
	void SetCacheSize(DWORD dwCacheSize);
	BOOL Clean(BOOL bTruncateFile = TRUE);
	void Log(LPCTSTR lpText);
	BOOL Flush();
	CString GetFilePath() const;
	BOOL SetFilePath(LPCTSTR lpPath);

private:

	CString m_sFile;
	DWORD m_dwCacheSize;
	CString m_sContents;
	BOOL m_bIsWriting;
	CString m_sTemp;
	CString m_sPrevious;
};

#endif // __LOGGER_H__
