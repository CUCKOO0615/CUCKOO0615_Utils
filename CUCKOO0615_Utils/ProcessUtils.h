//************************************************
//◇作者：CUCKOO0615
//◇日期：2014/10/15 15:31:20
//◇说明：进程相关操作
//************************************************
#pragma once

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#include <vector>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#pragma comment(lib,"psapi.lib")

class ProcessUtils
{
public:
    /*
    ** 获取当前任务管理器中所有进程PROCESSENTRY32结构体的列表
    ** @param vecPsEntry: 操作成功返回PROCESSENTRY32结构体列表
    ** @return: 操作成功返回true, 失败返回false, 由GetErrMsg获取错误信息
    */
    static bool GetProcessEntryList(std::vector<PROCESSENTRY32>& vecPsEntry);

    /*
    ** 获取指定PROCESSENTRY32结构体中的进程ID信息
    ** @param psEntry: 包含进程信息的PROCESSENTRY32结构体
    */
    static DWORD GetProcessID(const PROCESSENTRY32& psEntry);

    /*
    ** 终止拥有指定进程ID的进程
    ** @param dwPsID: 指定的进程ID
    ** @return: 操作成功返回true, 失败返回false, 由GetErrMsg获取错误信息
    */
    static bool KillProcess(DWORD dwPsID);

    /*
    ** 检查指定进程是否存在
    ** @param szPsImageName: 进程映像名
    ** @return: 操作成功返回true, 失败返回false, 由GetErrMsg获取错误信息
    */
    static bool CheckProcessExist(const TCHAR* szPsImageName);

    /*
    ** 获取指定进程的本地文件路径
    ** @param dwPsID: 指定进程的ID
    ** @param szNtPath: 返回进程的本地文件路径
    ** @return: 操作成功返回true, 失败返回false, 由GetErrMsg获取错误信息
    */
    static bool GetProcessFilePathByID(DWORD dwPsID, TCHAR* szNtPath);

    /*
    ** 获取指定设备目录的逻辑目录
    ** @param pszDosPath: 设备目录
    ** @param pszNtPath: 逻辑目录字符串缓冲区指针, 需保留足够的大小
    ** @return: 操作成功返回true, pszNtPath指向的缓冲区中包含转换后的逻辑目录
    ** @return: 操作失败返回false, 由GetErrMsg获取错误信息
    */
    static bool DosPath2NtPath(const TCHAR* pszDosPath, TCHAR* pszNtPath);

    // 错误信息
    static const char* GetLastErrMsg() { return _szErrMsg; }

private:
    static char _szErrMsg[ERRMSG_LENGTH];
};

