
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：文件处理
//*************************************************
#pragma once

#ifndef OUT
#define OUT
#endif

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

namespace FileUtils
{
    /*
    ** 获取文件字节大小
    ** @Param szFilePath: 文件路径
    ** @Param nFileSize: 文件字节大小
    ** @Ret : 操作成功返回0，操作失败返回由GetLastError()获取的错误码
    */
    DWORD GetFileSize(const char* szFilePath, OUT __int64 & nFileSize);

    /*
    ** 删除文件
    ** @Param szFile: 文件路径
    ** @Param bRecycle: true-删除到回收站，false-永久删除
    ** @Ret : 操作成功返回true，失败返回false，由GetLastError()获取错误码
    */
    bool DelFile(const char* szFile, bool bRecycle);

    /*
    ** 设置文件最后修改时间
    ** @Param szFilePath: 文件路径
    ** @Param stuFileTime: 时间
    ** @Ret : 操作成功返回0，操作失败返回由GetLastError()获取的错误码
    */
    DWORD SetLastWriteTime(const char* szFilePath, const FILETIME& stuFileTime);
    DWORD SetLastWriteTime(const char* szFilePath, const SYSTEMTIME& stuSysTime);
    //DWORD SetLastWriteTime(const char* szFilePath, const time_t t);
#ifdef __AFXWIN_H__
    DWORD SetLastWriteTime(const char* szFilePath, const CTime& clsTime);
#endif

};


