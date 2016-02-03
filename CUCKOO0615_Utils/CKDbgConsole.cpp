#include "StdAfx.h"
#include "CKDbgConsole.h"
#include <time.h>

CKDbgConsole::CKDbgConsole():m_bInited(false),m_bShowLogTime(true),m_hConsoleHandle(NULL)
{
	::InitializeCriticalSectionAndSpinCount(&m_csWriteLock, 8000);
}

CKDbgConsole::~CKDbgConsole()
{
	::DeleteCriticalSection(&m_csWriteLock);
	ExitConsole();
}

bool CKDbgConsole::ShowConsole()
{
	if(m_bInited)		        return true;
	if (0 == ::AllocConsole())	return false;

	HWND hHwnd = ::GetConsoleWindow();
	if(hHwnd)
	{
		HMENU hMenu = ::GetSystemMenu( hHwnd, FALSE);
		if(hMenu) ::RemoveMenu( hMenu, 0xF060, 0);
		m_hConsoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	}

	m_bInited = (hHwnd && m_hConsoleHandle);
	if(!m_bInited) ::FreeConsole();
	return m_bInited;
}

void CKDbgConsole::ExitConsole()
{
	if(!m_bInited)
		return;
	::FreeConsole();
	::CloseHandle(m_hConsoleHandle);
	m_hConsoleHandle = NULL;
	m_bInited = false;
}

CKDbgConsole & CKDbgConsole::GetInstance()
{
    static CKDbgConsole me;
    return me;
}

void CKDbgConsole::WriteLine(const char * pszMsg)
{
    WriteLine(const_cast<char*>(pszMsg), strlen(pszMsg)+1);
}
void CKDbgConsole::WriteLine(char * pBuff, size_t nBuffSize)
{
    if (!m_bInited || !pBuff || !nBuffSize)
        return;
	::EnterCriticalSection(&m_csWriteLock);
    DWORD nWriteNum = 0;
    if (m_bShowLogTime)
    {
        char szBuf[30] = { 0 };
		time_t lt;
		time(&lt);
		::strftime(szBuf, 30, "[%Y/%m/%d %H:%M:%S]", localtime(&lt));
        ::WriteConsoleA(m_hConsoleHandle, szBuf, strlen(szBuf)+1, &nWriteNum, NULL);
    }
    ::WriteConsoleA(m_hConsoleHandle, pBuff, nBuffSize, &nWriteNum, NULL);
    ::WriteConsoleA(m_hConsoleHandle, "\n", 1, &nWriteNum, NULL);
	::LeaveCriticalSection(&m_csWriteLock);
    return;
}

void CKDbgConsole::Write(const char * pszMsg)
{
    Write(const_cast<char*>(pszMsg), strlen(pszMsg));
}
void CKDbgConsole::Write(char* pBuff, size_t nBuffSize)
{
    if (!m_bInited || !pBuff || !nBuffSize)
        return;

	::EnterCriticalSection(&m_csWriteLock);
    DWORD nWriteNum = 0;
    ::WriteConsoleA(m_hConsoleHandle, pBuff, nBuffSize, &nWriteNum, NULL);
	::LeaveCriticalSection(&m_csWriteLock);
    return;
}

