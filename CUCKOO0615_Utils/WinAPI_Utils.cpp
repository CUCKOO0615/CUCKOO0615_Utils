#include "StdAfx.h"
#include "WinAPI_Utils.h"



HANDLE WinAPI_Utils::CreateMutex(BOOL bInitialOwner, LPCTSTR szName)
{
    return ::CreateMutex(NULL, bInitialOwner, szName);
}

bool WinAPI_Utils::MutexIsExist(LPCTSTR szName)
{
    HANDLE hlTmp = ::CreateMutex(NULL, FALSE, szName);
    if (ERROR_ALREADY_EXISTS == ::GetLastError())
        return true;
    if (FALSE == ::ReleaseMutex(hlTmp))
        return true;
    ::CloseHandle(hlTmp);
    return false;
}

bool WinAPI_Utils::ReleaseMutex(HANDLE hMutex)
{
    if (FALSE == ::ReleaseMutex(hMutex))
        return false;
    return true;
}

//////////////////////////////////////////////////////////////////////////

HLOCAL WinAPI_Utils::GetLastErrMsg(DWORD dwErrCode, TCHAR*& szErrMsg)
{
    HLOCAL hLocal = NULL;
    DWORD dwSystemLocal = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
    DWORD dwRet = ::FormatMessage(dwFlags, NULL, dwErrCode, dwSystemLocal, (PTSTR)&hLocal, 0, NULL);

	if (!dwRet) //À´×ÔÍøÂçµÄ´íÎó
    {
        HMODULE hml = ::LoadLibraryEx(_T("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (NULL != hml)
        {
            dwFlags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER;
			dwRet = ::FormatMessage(dwFlags, hml, dwErrCode, dwSystemLocal, (PTSTR)&hLocal, 0, NULL);
            ::FreeLibrary(hml);
        }
    }
	if (dwRet && (NULL != hLocal))
    {
        szErrMsg = (TCHAR*)::LocalLock(hLocal);
    }
    else 
    {
        szErrMsg = NULL;
        if (NULL != hLocal)
        {
            ::LocalFree(hLocal);
            hLocal = NULL;
        }
    }
    return hLocal;
}

void WinAPI_Utils::FreeLastErrMsgBuffer(HLOCAL& hLocal)
{
    if (NULL != hLocal)
    {
        ::LocalFree(hLocal);
        hLocal = NULL;
    }
}


