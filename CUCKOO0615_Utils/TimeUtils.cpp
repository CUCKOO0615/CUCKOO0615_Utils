
#include "TimeUtils.h"

struct TM_SPLIT
{
    char YEAR[5], MON[3], DAY[3];
    char HOUR[3], MIN[3], SEC[3];
};

time_t TimeUtils::Str2Time_t(const char* szStr)
{
    if (!szStr || strlen(szStr) >= TIME_STR_LENGTH)
        return 0;

    TM_SPLIT s;
    memcpy(&s, szStr, TIME_STR_LENGTH);

    s.YEAR[4] = 0x00, s.MON[2] = 0x00, s.DAY[2] = 0x00;
    s.HOUR[2] = 0x00, s.MIN[2] = 0x00, s.SEC[2] = 0x00;

    tm tmTmp;
    tmTmp.tm_year = atoi(s.YEAR) - 1900;
    tmTmp.tm_mon = atoi(s.MON) - 1;
    tmTmp.tm_mday = atoi(s.DAY);
    tmTmp.tm_hour = atoi(s.HOUR);
    tmTmp.tm_min = atoi(s.MIN);
    tmTmp.tm_sec = atoi(s.SEC);

    return mktime(&tmTmp); 
}

tm TimeUtils::Str2Tm(const char* szStr)
{
    time_t lRet = Str2Time_t(szStr);
    return *localtime(&lRet);   //可以自动计算tm中的tm_wday,tm_yday,tm_isdst三个成员
}

void TimeUtils::Time_t2Str(time_t lTime, char * szBuf, size_t nBufSize /*= TIME_STR_LENGTH */)
{
    if (nBufSize < TIME_STR_LENGTH)
        return;
    strftime(szBuf, TIME_STR_LENGTH, "%Y/%m/%d %H:%M:%S", localtime(&lTime));
}

void TimeUtils::Tm2Str(tm tmTime, char * szBuf, size_t nBufSize /*= TIME_STR_LENGTH */)
{
    time_t lTime = mktime(&tmTime);
    TimeUtils::Time_t2Str(lTime, szBuf, nBufSize);
}

