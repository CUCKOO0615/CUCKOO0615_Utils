//************************************************
//◇作者：CUCKOO0615
//◇日期：2015/04/19
//◇说明：使用Critical Section实现的同步锁
//*************************************************

#pragma once
#include <windows.h>

#if _MSC_VER<=1200
#define  _WIN32_WINNT 0x0500
#endif

/*
** 手动锁
** 例:
** SyncLock sl;
** sl.Lock();
** ..do sth..
** sl.Unlock();
*/
class SyncLock
{
public:
	void Lock()                     { ::EnterCriticalSection(&m_cs); }
	void Unlock()                   { ::LeaveCriticalSection(&m_cs); }
public:
    SyncLock(DWORD dwSpinCount = 0)
    {
        if (dwSpinCount)
            ::InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount);
        else
            ::InitializeCriticalSection(&m_cs);
    }
	~SyncLock()                     { ::DeleteCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

class SyncMutex
{
public:
    void Lock(DWORD dwTimeOut = INFINITE)    { if (MutexValid()) ::WaitForSingleObject(m_hlMutex, dwTimeOut); }
    void Unlock()                            { if (MutexValid()) ::ReleaseMutex(m_hlMutex); }
public:
    SyncMutex(const char* szMutexName = NULL) :
        m_hlMutex(INVALID_HANDLE_VALUE)      {
        m_hlMutex = ::CreateMutexA(NULL, FALSE, szMutexName);
    }
    ~SyncMutex()                             { if (MutexValid()) ::CloseHandle(m_hlMutex); }
private:
    HANDLE m_hlMutex;
    bool MutexValid() { return (m_hlMutex && (INVALID_HANDLE_VALUE != m_hlMutex)); }
};

/*
** 自动锁
** 例:
** SyncLock sl;
** SyncLockGuard slg(&sl);
** ..do sth..
** 可手动调用slg.ManualUnlock(), 或等待出作用域时自动解锁
*/
class SyncLockGuard
{
public:
    SyncLockGuard(SyncLock* pLock):m_pLock(pLock)   { if (m_pLock) m_pLock->Lock(); }
    ~SyncLockGuard()                                { if (m_pLock) m_pLock->Unlock(); m_pLock = NULL; }
    void ManualUnlock()                             { if (m_pLock) m_pLock->Unlock(); m_pLock = NULL; }
private:
    SyncLock* m_pLock;
};

/*
**  SyncLock sl;
**  AUTO_LOCK_BEG(sl);
**  ..do sth..
**  AUTO_LOCK_END;
*/
#define AUTO_LOCK_BEG(sl) {  \
    SyncLockGuard slg_##sl(&sl)
#define AUTO_LOCK_END     }

/*
**  SyncLock sl;
**  {
**      AUTO_LOCK(sl);
**      ..do sth..
**  }
*/
#define AUTO_LOCK(sl) SyncLockGuard slg_##sl(&sl)

/*
** 自动锁
** 例:
** SyncMutex sm;
** SyncMutexGuard smg(&sm);
** ..do sth..
** 可手动调用smg.ManualUnlock(), 或等待出作用域时自动解锁
*/
class SyncMutexGuard
{
public:
    SyncMutexGuard(SyncMutex* pMutex):m_pMutex(pMutex)  { if (m_pMutex) m_pMutex->Lock(); }
    ~SyncMutexGuard()                                   { if (m_pMutex) m_pMutex->Unlock(); m_pMutex = NULL; }
    void ManualUnlock()                                 { if (m_pMutex) m_pMutex->Unlock(); m_pMutex = NULL; }
private:
    SyncMutex* m_pMutex;
};
