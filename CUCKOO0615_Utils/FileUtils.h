
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月10日
//◇说明：文件处理
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

#include <string.h>

class FileUtils
{

private:
    char m_szErrMsg[ERRMSG_LENGTH];
    void SetErrMsg(const char* szErrMsg)   { strcpy(m_szErrMsg, szErrMsg); }
    void SetErrMsgEx(const char* szErrMsg) { strcat(m_szErrMsg, szErrMsg); }

public:
    FileUtils(void);
    ~FileUtils(void);

    /*
    ** 获取文件字节大小
    ** @param szFilePath: 文件路径
    ** @return: 0表示失败或错误或文件大小就是0
    */
    __int64 GetFileSize(const char* szFilePath);

    //获取错误信息
    const char* GetErrMsg() { return m_szErrMsg; }
};

