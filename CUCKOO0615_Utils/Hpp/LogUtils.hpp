//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月24日
//◇说明：日志单例类
//*************************************************
#pragma once
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <direct.h>
#include <ctime>
#include <sys/stat.h>
#include <assert.h>

#ifndef CK_LOG_UTILS
#define CK_LOG_UTILS
#if _MSC_VER<1600
#define WRITE_LOG_INFO(a)  (LogUtils::GetInstance().Recording(LL_INFO, a))
#define WRITE_LOG_WARN(a)  (LogUtils::GetInstance().Recording(LL_WARN, a))
#define WRITE_LOG_ERROR(a) (LogUtils::GetInstance().Recording(LL_ERROR, a))
#else
#define WRITE_LOG_INFO(a,...)  (LogUtils::GetInstance().Recording(LL_INFO, a, __VA_ARGS__))
#define WRITE_LOG_WARN(a,...)  (LogUtils::GetInstance().Recording(LL_WARN, a, __VA_ARGS__))
#define WRITE_LOG_ERROR(a,...) (LogUtils::GetInstance().Recording(LL_ERROR, a, __VA_ARGS__))
#endif
#define WRITE_LOG_LINE(a)  (LogUtils::GetInstance().SperateLine(a))
#endif

typedef enum emLogLevel
{
    LL_INFO, LL_WARN, LL_ERROR
}LOG_LEVEL;

class LogUtils
{
    static const int LOGPATH_BUFFLEN = MAX_PATH;
    static const int TIMESTR_BUFFLEN = 100;
    static const int LOGINDEX_MAX    = 99999999;
    static const int LOGFILESIZE_MAX = 0xA00000; //10M

    FILE * m_pLogFile;
    bool m_isInited;
    int m_nLogIndex;
    char m_szLogPath[LOGPATH_BUFFLEN];
	char m_szCurTime[TIMESTR_BUFFLEN];
    struct _stat m_stuFileState;
	CRITICAL_SECTION m_csSync;

    LogUtils(): m_pLogFile(NULL), m_isInited(false), m_nLogIndex(0)
    {
		::memset(m_szLogPath, 0x00, LOGPATH_BUFFLEN);
		::memset(m_szCurTime, 0x00, TIMESTR_BUFFLEN);
    }
    LogUtils(const LogUtils& lu){}

public:
	~LogUtils()
	{
        if(m_isInited) ::DeleteCriticalSection(&m_csSync);
	}

private:
    void SwitchLogFileByFileSize()
    {
        if (!m_isInited) return;
        if (0 != ::_access(m_szLogPath, 0)) return;

        while(true)
        {
            int nRet = ::_stat(m_szLogPath, &m_stuFileState);
            _off_t nFileSize = (0 == nRet) ? m_stuFileState.st_size : 0;

            //单个日志10M分文件
            if(nFileSize < LOGFILESIZE_MAX)
                return;

            ++m_nLogIndex;
            if (m_nLogIndex >= LOGINDEX_MAX)
                return;

            char numBuf[16] = { 0 };
            ::sprintf(numBuf, "%d.txt", m_nLogIndex);

            int i = ::strlen(m_szLogPath);
            if (LOGPATH_BUFFLEN < (i + ::strlen(numBuf)))
                return;
            for (; i != -1; --i)
            {
                if (m_szLogPath[i] == '_')
                {
                    m_szLogPath[i + 1] = 0x00;
                    break;
                }
            }
            ::strcat(m_szLogPath, numBuf);
        }
    }

    bool CreateMultiDir(const char* szDirPath)
    {
		assert(szDirPath);
        size_t nLength = strlen(szDirPath);
		assert(nLength);

        char szTmp[LOGPATH_BUFFLEN] = { 0 };
        for (int i = 0; i != nLength; ++i)
        {
            if ('/' == szDirPath[i] || 
                '\\' == szDirPath[i]||
                '\0' == szDirPath[i])
            {
                ::memcpy(szTmp, szDirPath, i);
                if (0 == ::_access(szTmp, 0))
                    continue;
                else if (0 != ::_mkdir(szTmp))
                    return false;
            }
        }
        return true;
    }

	const char* CurrentTimeToString()
	{
		time_t t;
		time(&t);
		::memset(m_szCurTime, 0x00, TIMESTR_BUFFLEN);
		::strftime(m_szCurTime, TIMESTR_BUFFLEN, "%Y/%m/%d %H:%M:%S", localtime(&t));
		return m_szCurTime;
	}

	const char* LogLevelToString(LOG_LEVEL emLL)
	{
		if (LL_INFO == emLL)
			return "INFO";
		else if (LL_WARN == emLL)
			return "WARN";
		else
			return "ERROR";
	}

	void SafeCloseLogFile()
	{
		int i = 0;
		while (i < 5)
		{
			if (0 == ::fclose(m_pLogFile))
				break;
			else
			{
				Sleep(100);
				++i;
			}
		}
		m_pLogFile = NULL;
	}

public:
    // 获取单例
    static LogUtils & GetInstance()
    {
        static LogUtils m_instance;
        return m_instance;
    }

    /*
    ** 初始化日志类
    ** @param szDirPath: 设置日志目录
    ** @param szLogNamePrefix:	日志文件名的前缀
    */
    bool InitLogUtils(
        const char* szDirPath = ".\\Log\\",
        const char* szLogNamePrefix = "Log")
    {
        if (LOGPATH_BUFFLEN <= ::strlen(szDirPath)
            || !CreateMultiDir(szDirPath)
            || 0 != ::_access(szDirPath, 0))
            return false;

        ::memset(m_szLogPath, 0x00, LOGPATH_BUFFLEN);
        ::strcpy(m_szLogPath, szDirPath);

        int nPos = ::strlen(szDirPath) - 1;
        if (m_szLogPath[nPos] != '\\' && m_szLogPath[nPos] != '/')
        {
            m_szLogPath[nPos + 1] = '\\';
            m_szLogPath[nPos + 2] = 0x00;
        }

        int nPathLength = ::strlen(m_szLogPath) + ::strlen(szLogNamePrefix) + ::strlen("_0.txt");
        if (LOGPATH_BUFFLEN < nPathLength)
            return false;
        ::strcat(m_szLogPath, szLogNamePrefix);
        ::strcat(m_szLogPath, "_0.txt");

		if (!m_isInited)
		{
			BOOL bRet = ::InitializeCriticalSectionAndSpinCount(&m_csSync, 8000);
			if (FALSE == bRet)
				return false;
		}
        m_isInited = true;
        return true;
    }

    /*
    ** 将指定的字符串写入日志
    ** @param wszRec: 输出的日志,支持格式化输出
    */
    void Recording(LOG_LEVEL emLL, const wchar_t* wszRec, ...)
    {
        va_list argsList;
        va_start(argsList, wszRec);
        wchar_t pBuff[512] = { 0 };
        ::vswprintf(pBuff, wszRec, argsList);
        va_end(argsList);

        char* pBuffMultiBytes = NULL;
        do 
        {
            int nBufLen = ::WideCharToMultiByte(CP_ACP, 0, pBuff, -1, NULL, 0, NULL, NULL);
            pBuffMultiBytes = new char[nBufLen];
            if (!pBuffMultiBytes)
                break;
            ::memset(pBuffMultiBytes, 0, nBufLen);
            if (!::WideCharToMultiByte(CP_ACP, 0, pBuff, -1, pBuffMultiBytes, nBufLen, NULL, NULL))
                break;
            Recording(emLL, pBuffMultiBytes);
        } while (0);
        if(pBuffMultiBytes) 
            delete[] pBuffMultiBytes;
        return;
    }

    /*
    ** 将指定的字符串写入日志
    ** @param szRec: 输出的日志,支持格式化输出
    */
    void Recording(LOG_LEVEL emLL, const char* szRec, ...)
    {
		if (!m_isInited) InitLogUtils();
        if (!m_isInited) return;
		::EnterCriticalSection(&m_csSync);
        SwitchLogFileByFileSize();

        m_pLogFile = ::fopen(m_szLogPath, "a+");
        if (m_pLogFile)
        {
			const char* arrLogLevel[] = { "INFO", "WARN", "ERROR" };
			::fprintf(m_pLogFile, " [%s %s]: ", CurrentTimeToString(), arrLogLevel[emLL]);

			va_list argsList;
			va_start(argsList, szRec);
			::vfprintf(m_pLogFile, szRec, argsList);
			va_end(argsList);

			::fprintf(m_pLogFile, "\n");
			SafeCloseLogFile();
        }
		::LeaveCriticalSection(&m_csSync);
    }

    /*
    ** 输出一条60个字符长的分割线
    ** @param ch：组成分割线的字符
    */
    void SperateLine(char ch = '-')
    {
		if (!m_isInited) InitLogUtils();
        if (!m_isInited) return;
		::EnterCriticalSection(&m_csSync);
        SwitchLogFileByFileSize();

        m_pLogFile = ::fopen(m_szLogPath, "a+");
        if (m_pLogFile)
        {
            char tmpBuf[61] = { 0 };
            ::memset(tmpBuf, ch, 60);
            ::fprintf(m_pLogFile, "%s\n", tmpBuf);
			SafeCloseLogFile();
        }
		::LeaveCriticalSection(&m_csSync);
    }
};
