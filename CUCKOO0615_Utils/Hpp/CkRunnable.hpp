//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/02/03
//��˵����ɽկJAVA��Runnable�ӿ�
//*************************************************

#include <windows.h>
#include <process.h>

class CkRunnable
{
	//////////////////////////////////////////////////////////////////////////
public:
	/*
	** ���ڱ�����̳е��߳���ں���,���������û����е���
	** @Param pThreadArgs: Ҫ�����̵߳ĸ��Ӳ���,�μ�SetThreadArgs����
	** @Ret: �߳��˳���,����GetThreadExitCode()������ȡ
	*/
	virtual UINT Running(void* pThreadArgs) = 0;

	/*
	** �����߳���ں���Running(void* pThreadArgs)��ʵ��
	** @Param pThreadArgs: 
	** ** ָ��Ҫ��ΪRunning������ʵ�εĶ���
	** ** ʹ�þֲ�����ʱ�����
	*/
	void SetThreadArgs(void* pThreadArgs);

	//�����߳�
	bool  Start();

	//��ȡ�߳̾��
	HANDLE GetThreadHandle() const;

	//��ȡ�߳�ID
	UINT   GetThreadId() const;

	/*
	** ��ȡ�߳��˳���,��Running�����ķ���ֵ
	** @Ret: 
	** ** �߳����˳�,������Ӧ�˳���;
	** ** �߳�������,���س���STILL_ACTIVE;
	** ** ����ʧ�ܷ���0xffffff;
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