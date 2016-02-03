//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/02/03
//◇说明：山寨JAVA的Runnable接口
//*************************************************

#include <windows.h>
#include <process.h>

class CkRunnable
{
public:
	//用于被子类继承
	//的线程入口函数
	virtual UINT Running() = 0;

	//启动线程
	bool  Start();

	//获取线程句柄
	HANDLE GetThreadHandle() const;

	//获取线程ID
	UINT   GetThreadId() const;

	//获取线程退出码
	DWORD  GetThreadExitCode() const;

public:
	CkRunnable() :m_hThreadHandle(0), m_uThreadID(0){}
	virtual ~CkRunnable(){}

private:
	UINT m_uThreadID;
	HANDLE m_hThreadHandle;

	static UINT __stdcall ThreadEntry(void* pArgList)
	{
		return ((CkRunnable*)pArgList)->Running();
	}
};

//////////////////////////////////////////////////////////////////////////
//inline
inline bool CkRunnable::Start()
{
	m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, CkRunnable::ThreadEntry, (void*)this, 0, &m_uThreadID);
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