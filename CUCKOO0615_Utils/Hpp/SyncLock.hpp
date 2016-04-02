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
    SyncLock()                      { ::InitializeCriticalSection(&m_cs); }
    SyncLock(DWORD dwSpinCount)     { ::InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount); }
    ~SyncLock()                     { ::DeleteCriticalSection(&m_cs); }
private:
    CRITICAL_SECTION m_cs;
	SyncLock(SyncLock&){};
};

/*
** 自动锁
** 例:
** SyncLock sl;
** SyncLockGuard slg(&sl);
** ..do sth..
** 可手动调用slg.ManualUnlock()
** 或出作用域时自动解锁
*/
class SyncLockGuard
{
public:
    SyncLockGuard(SyncLock* pLock) :m_pLock(pLock)   { if (m_pLock) m_pLock->Lock(); }
    ~SyncLockGuard()                                 { if (m_pLock) m_pLock->Unlock(); }

    void ManualUnlock()
    {
        if (m_pLock) m_pLock->Unlock();
        m_pLock = NULL;
    }

private:
    SyncLock* m_pLock;
};