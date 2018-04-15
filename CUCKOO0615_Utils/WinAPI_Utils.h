
#pragma once
#include <windows.h>

namespace WinAPI_Utils
{
    namespace MappingFileUtils
    {
        enum MappingType
        {
            MT_ReadOnly,
            MT_ReadWrite
        };
        struct MappingFileInfo;

        /*
        ** 创建内存映射文件
        ** @Param szFilePath: 文件路径
        ** @Param emMT: 文件读写属性 MT_ReadOnly/MT_ReadWrite
        ** @Param mfi: 文件信息结构体, 成员pbFile即为读写指针, mfi析构时会自动释放资源
        ** @Param dwFileSize: 创建新文件时,需指定一个非0的文件大小, 不创建新文件时该参数被忽略
        ** @Ret : 操作成功返回0, 内部API调用失败后, DWORD GetLastError()的返回值
        */
        DWORD CreateMappingFile(const TCHAR* szFilePath, MappingType emMT, MappingFileInfo& mfi, DWORD dwFileSize = 0);


    }

    namespace MutexUtils
    {
        /*
           ** 创建一个默认权限级别(不能被子进程继承)的互斥量对象
           ** @param bInitialOwner: 创建互斥量后是否由本线程立即获得所有权, TRUE获得, FALSE放弃
           ** @param szName: 命名互斥量的名字,默认为匿名
           ** @return: 成功返回有效的互斥量对象句柄, 失败返回NULL, 由GetLastError获取错误码
           */
        HANDLE CreateMutex(BOOL bInitialOwner, LPCTSTR szName = NULL);

        /*
        ** 释放指定互斥量对象的所有权
        ** @param hMutex: 互斥量对象句柄
        ** @return: 操作成功返回true, 操作失败返回false, 由GetLastError获取错误码
        */
        bool ReleaseMutex(HANDLE hMutex);

        /*
        ** 检查指定名字的互斥量对象是否存在
        ** @param szName: 指定的互斥量对象名
        ** @return: 互斥量已存在返回true, 否则返回false
        */
        bool MutexIsExist(LPCTSTR szName);
    }

    //////////////////////////////////////////////////////////////////////////

    /*
    ** 获取GetLastError()返回的错误号所代表的错误信息
    ** @Param dwErrCode: 由GetLastError()返回的错误号
    ** @Param szErrMsg: 指向错误信息字符串的指针,不需要分配空间(new)
    ** @Ret: 由系统分配的用于容纳错误信息的内存缓冲区句柄
    ** - PS: 该句柄需要由FreeLastErrMsgBuffer函数手动释放,否则会有内存泄漏.
    ** -   : 操作成功,szErrMsg指向错误信息字符串; 操作失败,szErrMsg指向NULL.
    */
    HLOCAL GetLastErrMsg(DWORD dwErrCode, TCHAR*& szErrMsg);

    /*
    ** 释放GetLastErrMsg函数返回的内存句柄
    ** @Param hLocal: 内存句柄
    */
    void FreeLastErrMsgBuffer(HLOCAL& hLocal);

    //////////////////////////////////////////////////////////////////////////
    struct MappingFileUtils::MappingFileInfo
    {
        HANDLE hFileHandle;
        HANDLE hFileMappingHandle;
        LPBYTE pbFile;

        MappingFileInfo() : hFileHandle(NULL), hFileMappingHandle(NULL), pbFile(NULL){}
        ~MappingFileInfo()
        {
            if (hFileHandle)
                ::CloseHandle(hFileHandle);
            if (hFileMappingHandle)
                ::CloseHandle(hFileMappingHandle);
            if (pbFile)
                ::UnmapViewOfFile(pbFile);
        }
    };
};

