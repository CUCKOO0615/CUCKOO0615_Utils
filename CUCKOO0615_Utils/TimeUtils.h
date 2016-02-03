
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵����ʱ�����ش���
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
    ** �ַ���תtime_t
    ** @param szStr: ʱ���ַ���, ��ʽ��YYYY/MM/DD HH:MM:SS
    ** @return: ת���õ���time_tֵ
    */
    time_t Str2Time_t(const char* szStr);

    /*
    ** �ַ���תtm
    ** @param szStr: ʱ���ַ���, ��ʽ��YYYY/MM/DD HH:MM:SS
    ** @return: ת���õ���tm�ṹ
    */
    tm Str2Tm(const char* szStr);

    /*
    ** time_tת�ַ���
    ** @param lTime: time_t��ʱ��ֵ
    ** @param szBuf: �ַ���������, �ɹ�ת���󱣴��ʾʱ����ַ���,��ʽ YYYY/MM/DD HH:MM:SS
    ** @param nBufSize: �ַ�����������С, ����Ϊ TIME_STR_LENGTH, ����ִ��ת��
    */
    void Time_t2Str(time_t lTime, char * szBuf, size_t nBufSize = TIME_STR_LENGTH);

    /*
    ** time_tת�ַ���
    ** @param tmTime: tm��ʱ��ֵ
    ** @param szBuf: �ַ���������, �ɹ�ת���󱣴��ʾʱ����ַ���,��ʽ YYYY/MM/DD HH:MM:SS
    ** @param nBufSize: �ַ�����������С, ����Ϊ TIME_STR_LENGTH, ����ִ��ת��
    */
    void Tm2Str(tm tmTime, char * szBuf, size_t nBufSize = TIME_STR_LENGTH);

}
