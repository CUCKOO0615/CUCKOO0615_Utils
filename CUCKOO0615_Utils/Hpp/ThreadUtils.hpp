//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年12月12日
//◇说明：封装的线程工具类
//*************************************************
#pragma once
#include <windows.h>
#include <process.h>

//线程入口
#define TU_DECLEAR_THREADENTRY(func_name) \
	unsigned int __stdcall func_name(void* pParam)

#define TU_IMPLEMENT_THREADENTRY(func_name) \
    unsigned int __stdcall func_name(void* pParam)

//
typedef unsigned int(_stdcall *TU_LPTHREADENTRY)(void*);

class ThreadUtils
{
    //////////////////////////////////////////////////////////////////////////
public:
    /*
    ** 设置要传递给线程的参数列表
    ** @param pParams: 指向要传递给线程的 参数列表的指针
    */
    void SetThreadParams(void* pParams);

    /*
    ** 启动线程
    ** @param pThreadEntry: 指向线程入口函数的指针
    ** @return: 操作成功返回true, 操作失败返回false
    */
    bool Run(TU_LPTHREADENTRY pThreadEntry);

    /*
    ** 获取线程退出码
    ** 线程未启动或无法获取退出码: 返回0xFFFFFFFF
    ** 线程正在运行: 返回STILL_ACTIVE (259)
    ** 线程已退出: 返回线程函数的返回值, 或由ExitThread或TerminateThread指定的错误码
    */
    DWORD GetExitCode() const;

    // 获取线程句柄
    HANDLE GetThreadHandle() const;

    // 获取线程ID
    unsigned int GetThreadID() const;

    // 等待线程退出，清理资源并重新初始化
    void WaitForCleaning();

    //////////////////////////////////////////////////////////////////////////

public:
    ThreadUtils() : m_uThreadID(0), m_hThreadHandle(0), m_pArgList(NULL)	{	}
    ~ThreadUtils(){}

private:
    unsigned int m_uThreadID;
    HANDLE m_hThreadHandle;
    void* m_pArgList;

    ThreadUtils(ThreadUtils& t){}
};

//////////////////////////////////////////////////////////////////////////
// inline
inline void ThreadUtils::SetThreadParams(void* pParams)
{
    m_pArgList = pParams;
}

inline bool ThreadUtils::Run(TU_LPTHREADENTRY pThreadEntry)
{
    m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, pThreadEntry, m_pArgList, 0, &m_uThreadID);
    return (m_hThreadHandle&&m_uThreadID);
}

inline DWORD ThreadUtils::GetExitCode() const
{
    if (!m_hThreadHandle || !m_uThreadID) return 0xFFFFFFFF;
    DWORD dwExitCode = 0;
    BOOL bRet = ::GetExitCodeThread(m_hThreadHandle, &dwExitCode);
    return bRet ? dwExitCode : 0xFFFFFFFF;
}

inline HANDLE ThreadUtils::GetThreadHandle() const
{
    return m_hThreadHandle;
}

inline unsigned int ThreadUtils::GetThreadID() const
{
    return m_uThreadID;
}

inline void ThreadUtils::WaitForCleaning()
{
    ::WaitForSingleObject(m_hThreadHandle, INFINITE);
    ::CloseHandle(m_hThreadHandle);
    m_hThreadHandle = 0;

    m_uThreadID = 0;

    m_pArgList = NULL;
}
