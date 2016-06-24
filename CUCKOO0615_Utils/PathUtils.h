//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：文件路径处理
//*************************************************
#pragma once
#include "stdafx.h"

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#include <windows.h>
#include <io.h>

#define CUCKOO0615_USE_STL
#ifdef CUCKOO0615_USE_STL
#include <vector>
#include <string>
#endif

class PathUtils
{
public:
    //////////////////////////////////////////////////////////////////////////
    //基于 STL 的函数
#ifdef CUCKOO0615_USE_STL
    /*
    ** 获取指定文件夹下的所有子文件夹全路径
    ** @param vecSubDirFullPaths: 子文件夹路径集合
    ** @param strDir: 根文件夹
    ** @param strWildcard: 通配符
    ** @param bEnterSubDir: 是否搜索子文件夹
    ** @param bEnterHiddenSubDir: 是否搜索隐藏子文件夹, 参数bEnterSubDir为true时生效
    ** @param nExceptFileTypes: 搜索时要排除的文件夹类型,
    ** - 可以是 _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH 中的一个或多个,用"|"连接
    */
    bool GetSubDirsInDir(
        std::vector<std::string>& vecSubDirFullPaths,
        std::string strDir,
        std::string strWildcard = "*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** 获取指定文件夹下的所有文件全路径
    ** @param vecFileFullPaths: 文件路径集合
    ** @param strDir: 根文件夹
    ** @param strWildcard: 通配符
    ** @param bEnterSubDir: 是否搜索子文件夹
    ** @param bEnterHiddenSubDir: 是否搜索隐藏子文件夹, 参数bEnterSubDir为true时生效
    ** @param nExceptFileTypes: 搜索时要排除的文件类型,
    ** - 可以是 _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH 中的一个或多个,用"|"连接
    */
    bool GetFilesInDir(
        std::vector<std::string>& vecFileFullPaths,
        std::string strDir,
        std::string strWildcard = "*.*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** 从全路径截取文件名或文件夹名
    ** @param strFileFullPath: 文件或文件夹的全路径
    */
    static std::string GetFileName(const std::string& strFileFullPath);
    
    /*
    ** 获取父目录路径
    ** @Param strPath: 指定的目录路径
    ** @Ret : 父目录路径
    */
    static std::string GetParentPath(const std::string& strPath);
    
    /*
    ** 将路径中的'/'全部替换为'\\'，并移除末尾的'\\'
    ** @Param strFilePath: 文件路径字符串
    */
    static void FixBackSlashInFilePath(std::string& strFilePath);

    /*
    ** 将路径中的'/'全部替换为'\\'，并在末尾补齐一个'\\'
    ** @Param strDirPath: 目录路径字符串    
    */
    static void FixBackSlashInDirPath(std::string& strDirPath);

    /*
    ** 将路径中的'/'全部替换为'\\'
    ** @Param strPath: 目录路径字符串
    */
    static void FixBackSlashInPath(std::string& strPath);

#endif

    /*
    ** 检查指定的路径是否存在(不区分文件和文件夹,较快)
    ** @param szPath: 要检查的路径
    */
    static bool PathIsExist(const char* szPath);

    /*
    ** 检查指定文件是否存在
    ** @param szFilePath: 要检查的路径
    */
    static bool FileIsExist(const char* szFilePath);

    /*
    ** 检查指定文件夹是否存在
    ** @param szDirPath: 要检查的路径
    */
    static bool DirIsExist(const char* szDirPath);

#ifdef __AFXWIN_H__
    /*
    ** 将路径中的'/'全部替换为'\'，并且合并连续的‘\’只留一个
    ** 如果不是一‘\’结尾，则在末尾补齐一个'\'
    ** @Param strDirPath: 目录路径字符串
    */
    static void FixBackSlash_DirPath(CString& strDirPath);

    /*
    ** 将路径中的'/'全部替换为'\'，并且合并连续的‘\’只留一个
    ** @Param strFilePath: 文件路径字符串
    */
    static void FixBackSlash_FilePath(CString& strFilePath);

    /*
    ** 将路径中的'\'全部替换为'/'，并且合并连续的‘/’只留一个
    ** 如果不是一‘/’结尾，则在末尾补齐一个'/'
    ** @Param strFtpRemotePath: 路径字符串
    */
    static void FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath);

    /*
    ** 将路径中的'\'全部替换为'/'，并且合并连续的‘/’只留一个
    ** @Param strFtpRemotePath: 路径字符串
    */
    static void FixSlash_FtpRemoteFilePath(CString& strFtpRemotePath);

#endif // __AFXWIN_H__

    /*
    ** 获取磁盘盘符列表
    ** @param arrLogicalDriveNames[26]: 磁盘盘符集合
    ** @return: 操作成功返回盘符数量(0 - 26), 操作失败返回 -1
    */
    static int GetDriveNames(char arrLogicalDriveNames[26]);

    /*
    ** 根据盘符获取磁盘类型
    ** @param chDriveName: 磁盘盘符
    ** @return: 未知驱动器/无效根目录/可移动设备/固定驱动器/网络驱动器/CD-ROM/RAM驱动器/未知设备
    */
    static const char* GetDriveType(char chDriveName);
    
    /*
    ** 获取当前进程的应用程序文件路径
    ** @Ret : 文件路径
    */
    static const char* GetAppFullPath();

    /*
    ** 获取当前进程的应用程序文件所在目录
    ** @Ret : 应用程序文件所在目录
    */
    static const char* GetAppDirectory();

    /*
    ** 从全路径截取文件名或文件夹名
    ** @param szFullPath: 文件或文件夹的全路径,最大MAX_PATH
    ** @param szName: 文件或文件夹名缓冲区指针
    ** @param nNameBufSize: 文件或文件夹名缓冲区大小
    */
    bool GetFileName(const char* szFullPath, char* szName, size_t nNameBufSize);
        
    /*
    ** 获取父目录
    ** @param szPath: 输入的路径,最长MAX_PATH-1
    ** @param szParentPath: 父目录缓冲区
    ** @param nParentPathBufSize: 父目录缓冲区长度,大小至少要大于szPath的长度
    */
    bool GetParentPath(const char* szPath, char* szParentPath, size_t nParentPathBufSize);
    
    /*
    ** 创建多级目录
    ** @param szPath: 指定的目录
    ** @return: 创建成功返回true
    */
    bool CreateMultiDirectory(const char* szPath);

    // 获取错误信息
    const char* GetErrMsg() { return m_szErrMsg; }
    
private:
    char m_szErrMsg[ERRMSG_LENGTH];

#ifdef CUCKOO0615_USE_STL
    bool GetFullPathsInDir(
        std::vector<std::string>& vecFullPaths,
        std::string strDir,
        const std::string& strWildcard,
        bool bEnterSubDir,
        bool bEnterHiddenSubDir,
        int nExceptFileTypes,
        bool bGetFiles,
        bool bGetDirs);
#endif

    //获取C风格字符串长度
    //返回值:空字符串或空指针返回0,否则返回字符串长度
    static size_t GetStringLength(const char* szString)
    {
        return ((NULL == szString) || 0 == strlen(szString)) ? 0 : strlen(szString);
    }

public:
    PathUtils() { ::memset(m_szErrMsg, 0x00, ERRMSG_LENGTH); }
    ~PathUtils() { }
};

