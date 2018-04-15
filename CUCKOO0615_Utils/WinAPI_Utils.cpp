#include "StdAfx.h"
#include "WinAPI_Utils.h"

//////////////////////////////////////////////////////////////////////////
// MutexUtils
HANDLE WinAPI_Utils::MutexUtils::CreateMutex(BOOL bInitialOwner, LPCTSTR szName)
{
    return ::CreateMutex(NULL, bInitialOwner, szName);
}

bool WinAPI_Utils::MutexUtils::MutexIsExist(LPCTSTR szName)
{
    HANDLE hlTmp = ::CreateMutex(NULL, FALSE, szName);
    if (ERROR_ALREADY_EXISTS == ::GetLastError())
        return true;
    ::CloseHandle(hlTmp);
    return false;
}

bool WinAPI_Utils::MutexUtils::ReleaseMutex(HANDLE hMutex)
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

//////////////////////////////////////////////////////////////////////////
// MappingFileUtils

DWORD WinAPI_Utils::MappingFileUtils::CreateMappingFile(
    const TCHAR* szFilePath, 
    MappingType emMT,
    WinAPI_Utils::MappingFileUtils::MappingFileInfo& mfi, 
    DWORD dwFileSize)
{
    bool bIsReadOnly = (MT_ReadOnly == emMT);

    HANDLE hFileHandle = ::CreateFile(szFilePath,
        (bIsReadOnly ? GENERIC_READ : GENERIC_ALL),
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_ARCHIVE,
        NULL);
    if (INVALID_HANDLE_VALUE == hFileHandle)
        return ::GetLastError();

    HANDLE hFileMappingHandle = ::CreateFileMapping(hFileHandle,
        NULL,
        (bIsReadOnly ? PAGE_READONLY : PAGE_READWRITE),
        0,
        (bIsReadOnly ? 0 : dwFileSize),
        NULL);
    if (NULL == hFileMappingHandle)
    {
        ::CloseHandle(hFileHandle);
        return ::GetLastError();
    }

    LPBYTE pbFile = (LPBYTE)::MapViewOfFile(hFileMappingHandle,
        (bIsReadOnly ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS),
        0, 0, 0);
    if (NULL == pbFile)
    {
        ::CloseHandle(hFileHandle);
        ::CloseHandle(hFileMappingHandle);
        return ::GetLastError();
    }

    mfi.hFileHandle = hFileHandle;
    mfi.hFileMappingHandle = hFileMappingHandle;
    mfi.pbFile = pbFile;
    return 0;
}
