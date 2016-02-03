
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：文件处理
//*************************************************

#include "FileUtils.h"
#include <string>
#include <windows.h>

FileUtils::FileUtils(void)
{
    memset(m_szErrMsg, 0x00, ERRMSG_LENGTH);
}

FileUtils::~FileUtils(void)
{
}

__int64 FileUtils::GetFileSize(const char* szFilePath)
{
    if (0 == strlen(szFilePath))
    {
        SetErrMsg("Invalid file path");
        return 0;
    }

    HANDLE hFileHandle = CreateFileA(szFilePath, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);

    if (INVALID_HANDLE_VALUE == hFileHandle)
    {
        SetErrMsg("Open file failed");
        if (0 == ::CloseHandle(hFileHandle))
            SetErrMsgEx(", and close hFileHandle failed, either");
        return 0;
    }

    LARGE_INTEGER largeInt;
    BOOL bRet = GetFileSizeEx(hFileHandle, &largeInt);
    if (0 == ::CloseHandle(hFileHandle))
        SetErrMsg("Close hFileHandle failed in GetFileSize");

    return largeInt.QuadPart;
}
