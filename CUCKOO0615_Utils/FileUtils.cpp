
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：文件处理
//*************************************************

#include "FileUtils.h"
#include <string>
#include <assert.h>
#include <time.h>

DWORD FileUtils::GetFileSize(const char* szFilePath, __int64 & nFileSize)
{
    assert(szFilePath);

    HANDLE hFileHandle = CreateFileA(szFilePath,
        GENERIC_READ,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, 
        OPEN_EXISTING,
        FILE_ATTRIBUTE_ARCHIVE, 
        NULL);

    if (INVALID_HANDLE_VALUE == hFileHandle) 
        return ::GetLastError();

    LARGE_INTEGER largeInt;
    ::GetFileSizeEx(hFileHandle, &largeInt);

    DWORD dwRet = ::GetLastError();
    ::CloseHandle(hFileHandle);
    return dwRet;
}

bool FileUtils::DelFile(const char* szFile, bool bRecycle)
{
    SHFILEOPSTRUCTA tagFO;
    ::memset(&tagFO, 0x00, sizeof(SHFILEOPSTRUCTA));

    tagFO.pFrom = szFile;
    tagFO.pTo = NULL;
    tagFO.wFunc = FO_DELETE;
    tagFO.hwnd = HWND_DESKTOP;
    tagFO.fFlags = FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMATION;
    if (bRecycle)
        tagFO.fFlags |= FOF_ALLOWUNDO;

    return (0 == ::SHFileOperationA(&tagFO));
}

DWORD FileUtils::SetLastWriteTime(const char* szFilePath, const FILETIME& stuFileTime)
{
    HANDLE handle = ::CreateFileA(szFilePath,
        FILE_WRITE_ATTRIBUTES,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_ARCHIVE,
        NULL);
    if (INVALID_HANDLE_VALUE == handle)
        return ::GetLastError();

    FILETIME ft1, ft2, ft3;
    do
    {
        if (0 == ::GetFileTime(handle, &ft1, &ft2, &ft3))
            break;
        if (0 == ::LocalFileTimeToFileTime(&stuFileTime, &ft3))
            break;
        if (0 == ::SetFileTime(handle, &ft1, &ft2, &ft3))
            break;
    } while (0);

    DWORD dwRet = ::GetLastError();
    ::CloseHandle(handle);
    return dwRet;
}

DWORD FileUtils::SetLastWriteTime(const char* szFilePath, const SYSTEMTIME& stuSysTime)
{
    FILETIME ft;
    if (!::SystemTimeToFileTime(&stuSysTime, &ft))
        return ::GetLastError();
    return SetLastWriteTime(szFilePath, ft);
}

// DWORD FileUtils::SetLastWriteTime(const char* szFilePath, const time_t t)
// {
//     tm* p = ::localtime(&t);
//     if (!p)
//         return -1;
// }

#ifdef __AFXWIN_H__
DWORD FileUtils::SetLastWriteTime(const char* szFilePath, const CTime& clsTime)
{
    SYSTEMTIME st;
    clsTime.GetAsSystemTime(st);
    return SetLastWriteTime(szFilePath, st);
}
#endif



