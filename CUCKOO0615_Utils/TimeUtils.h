
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：时间的相关处理
//*************************************************

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

#include <time.h>
#include <iostream>
#define TIME_STR_LENGTH 20

namespace TimeUtils
{
    /*
    ** 字符串转time_t
    ** @param szStr: 时间字符串, 格式：YYYY/MM/DD HH:MM:SS
    ** @return: 转换得到的time_t值
    */
    time_t Str2Time_t(const char* szStr);

    /*
    ** 字符串转tm
    ** @param szStr: 时间字符串, 格式：YYYY/MM/DD HH:MM:SS
    ** @return: 转换得到的tm结构
    */
    tm Str2Tm(const char* szStr);

    /*
    ** time_t转字符串
    ** @param lTime: time_t型时间值
    ** @param szBuf: 字符串缓冲区, 成功转换后保存表示时间的字符串,格式 YYYY/MM/DD HH:MM:SS
    ** @param nBufSize: 字符串缓冲区大小, 至少为 TIME_STR_LENGTH, 否则不执行转换
    */
    void Time_t2Str(time_t lTime, char * szBuf, size_t nBufSize = TIME_STR_LENGTH);

    /*
    ** time_t转字符串
    ** @param tmTime: tm型时间值
    ** @param szBuf: 字符串缓冲区, 成功转换后保存表示时间的字符串,格式 YYYY/MM/DD HH:MM:SS
    ** @param nBufSize: 字符串缓冲区大小, 至少为 TIME_STR_LENGTH, 否则不执行转换
    */
    void Tm2Str(tm tmTime, char * szBuf, size_t nBufSize = TIME_STR_LENGTH);

}
