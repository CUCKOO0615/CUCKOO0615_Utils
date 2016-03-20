//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：字符串处理
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

#define CUCKOO0615_USE_STL

#include <windows.h>
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
// 基于 STL 实现的函数
#ifdef CUCKOO0615_USE_STL
#include <string>
#include <vector>
using namespace std;

namespace StringUtils
{
    /*
    ** 移除左侧连续字符
    ** @param str: 源字符串
    ** @param chSpilter: 目标字符
    */
    void TrimLeft(string& str, char chSpilter = ' ');

    /*
    ** 移除右侧连续字符
    ** @param str: 源字符串
    ** @param chSpilter: 目标字符
    */
    void TrimRight(string& str, char chSpilter = ' ');

    /*
    ** 移除首尾连续字符
    ** @param str: 源字符串
    ** @param chSpilter: 目标字符
    */
    void Trim(string& str, char chSpilter = ' ');

    /*
    ** 替换字符串中指定的子串为另一个子串
    ** @param str: 源字符串
    ** @param oldStr: 要被替换的旧子串
    ** @param newStr: 用于替换的新子串
    */
    void ReplaceSpecifiedString(string& str, string oldStr, string newStr);

    /*
    ** 切分字符串到数组
    ** @param vecOut: 输出的字符串数组
    ** @param str: 输入的字符串
    ** @param chSpliter: 字符串切分符
    */
	void SplitStr2Array(vector<string>& vecOut, const string& str, char chSpliter, bool bSkipContinuous = true);

    /*
    ** 多字节字符转宽字节字符
    ** @param szMultibyteStr: 输入的多字节字符串
    ** @param strWide: 输出的宽字节字符串
    ** @param strErrMsg: 错误信息
    */
    bool StrConv_A2W(const char* szMultibyteStr, wstring& strWide, string& strErrMsg);

    /*
    ** 宽字节字符转多字节字符
    ** @param szWidestr: 输入的宽字节字符串
    ** @param szMultibyteStr: 输出的多字节字符串
    ** @param strErrMsg: 错误信息
    */
    bool StrConv_W2A(const wchar_t* szWidestr, string& szMultibyteStr, string& strErrMsg);
}
#endif
//////////////////////////////////////////////////////////////////////////

namespace StringUtils
{
    /*
    ** 移除左侧连续字符
    ** 不论字符串数组是否有\0结束均可使用
    ** @param szStr: 源字符串缓冲区,
    ** @param nBufSize: szStr缓冲区字节长度
    ** @param chSpilter: 目标字符
    */
    void TrimLeft(char * szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** 移除右侧连续字符
    ** 不论字符串数组是否有\0结束均可使用
    ** @param szStr: 源字符串缓冲区
    ** @param nBufSize: szStr缓冲区字节长度
    ** @param chSpilter: 目标字符
    */
    void TrimRight(char* szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** 移除首尾连续字符
    ** 不论字符串数组是否有\0结束均可使用
    ** @param szStr: 源字符串缓冲区
    ** @param nBufSize: szStr缓冲区字节长度
    ** @param chSpilter: 目标字符
    */
    void Trim(char* szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** 替换字符串中指定的子串为另一个子串
    ** 不论字符串数组szStr是否有\0结束均可使用
    ** @param szStr: 源字符串缓冲区,要确保缓冲区空间足够,
    ** - 如果替换操作后的字符串所需的缓冲区长度超出szStr所指向的缓冲区长度,
    ** - 不会引起报错或异常,替换后的字符串会占满预留的缓冲区,并在nBufSize处截断,
    ** - 这会导致szStr末尾的\0字节被覆盖掉,从而变成一个非\0结束的字符串数组.
    ** @param nBufSize: 源字符串缓冲区长度
    ** @param szOldStr: 要被替换的子串
    ** @param szNewStr: 替换的子串
    */
    void ReplaceSpecifiedString(char* szStr, size_t nBufSize, const char* szOldStr, const char* szNewStr);

    /*
    ** 判断字符串是否是合法的正数\负数
    ** @param szStr: 输入的字符串
    */
    bool IsDigitStr(const char* szStr);

    /*
    ** 二进制字节流转换为可读的16进制文本
    ** @param inBuff: 源二进制数据缓冲区
    ** @param nInBuffSize: 源二进制数据缓冲区大小
    ** @param outBuff: 输出文本缓冲区
    ** @param nOutBuffSize: 输出文本缓冲区大小
    ** - 由于一个二进制字节对应2个16进制文本字符,
    ** - 故nOutBuffSize不得小于nInBuffSize*2, 否则函数返回false
    */
    bool Hex2Text(const char* inBuff, size_t nInBuffSize, char* outBuff, size_t nOutBuffSize);
};

//////////////////////////////////////////////////////////////////////////
// 部分inline实现
#ifdef CUCKOO0615_USE_STL

inline void StringUtils::TrimLeft(string& str, char chSpilter /*= ' ' */)
{
    size_t nPos = str.find_first_not_of(chSpilter);
    if (string::npos == nPos)
    {
        str = "";
        return;
    }
    str.erase(0, nPos);
}

inline void StringUtils::TrimRight(string& str, char chSpilter /*= ' ' */)
{
    size_t nPos = str.find_last_not_of(chSpilter);
    if (string::npos == nPos)
    {
        str = "";
        return;
    }
    str.erase(nPos + 1);
}

inline void StringUtils::Trim(string& str, char chSpilter /*= ' ' */)
{
    size_t nPosFirst = str.find_first_not_of(chSpilter);
    if (string::npos != nPosFirst)
        str.erase(0, nPosFirst);
    else
    {
        str = "";
        return;
    }
    size_t nPosLast = str.find_last_not_of(chSpilter);
    if (string::npos != nPosLast)
        str.erase(nPosLast + 1);
}
#endif

