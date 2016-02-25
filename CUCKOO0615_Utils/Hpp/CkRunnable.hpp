//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/03
//◇说明：山寨JAVA的Runnable接口
//*************************************************

#include <windows.h>
#include <process.h>

class CkRunnable
{
	//////////////////////////////////////////////////////////////////////////
public:
	/*
	** 用于被子类继承的线程入口函数,不建议由用户自行调用
	** @Param pThreadArgs: 要传入线程的附加参数,参见SetThreadArgs函数
	** @Ret: 线程退出码,可由GetThreadExitCode()函数获取
	*/
	virtual UINT Running(void* pThreadArgs) = 0;

	/*
	** 设置线程入口函数Running(void* pThreadArgs)的实参
	** @Param pThreadArgs: 
	** ** 指向要做为Running函数的实参的对象
	** ** 使用局部对象时需谨慎
	*/
	void SetThreadArgs(void* pThreadArgs);

	//启动线程
	bool  Start();

	//获取线程句柄
	HANDLE GetThreadHandle() const;

	//获取线程ID
	UINT   GetThreadId() const;

	/*
	** 获取线程退出码,即Running函数的返回值
	** @Ret: 
	** ** 线程已退出,返回相应退出码;
	** ** 线程运行中,返回常数STILL_ACTIVE;
	** ** 操作失败返回0xffffff;
	*/
	DWORD  GetThreadExitCode() const;
	
	//////////////////////////////////////////////////////////////////////////
public:
	CkRunnable() :m_hThreadHandle(0), m_uThreadID(0)
	{
		m_arrThreadArgs[0] = (void*)this;
		m_arrThreadArgs[1] = NULL;
	}
	virtual ~CkRunnable()
	{
		if (m_hThreadHandle)
			::CloseHandle(m_hThreadHandle);
	}

private:
	UINT m_uThreadID;
	HANDLE m_hThreadHandle;
	void* m_arrThreadArgs[2];

	static UINT __stdcall ThreadEntry(void* pArgList)
	{
		void** pArgs = (void**)pArgList;
		return ((CkRunnable*)(pArgs[0]))->Running(pArgs[1]);
	}
};

//////////////////////////////////////////////////////////////////////////
//inline Implements

inline void CkRunnable::SetThreadArgs(void* pThreadArgs)
{
	m_arrThreadArgs[1] = pThreadArgs;
}

inline bool CkRunnable::Start()
{
	m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, CkRunnable::ThreadEntry, (void*)m_arrThreadArgs, 0, &m_uThreadID);
	return (m_hThreadHandle&&m_uThreadID);
}

inline HANDLE CkRunnable::GetThreadHandle() const 
{ 
	return m_hThreadHandle; 
}

inline UINT CkRunnable::GetThreadId() const 
{ 
	return m_uThreadID; 
}

inline DWORD CkRunnable::GetThreadExitCode() const
{
	if (!m_hThreadHandle || !m_uThreadID) return 0xffffff;
	DWORD dwExitCode = 0;
	BOOL bRet = ::GetExitCodeThread(m_hThreadHandle, &dwExitCode);
	return bRet ? dwExitCode : 0xffffff;
}