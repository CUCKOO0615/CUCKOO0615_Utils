//************************************************
//◇作者：CUCKOO0615
//◇日期：2014/11/19 16:05:53
//◇说明：配置文件工具
//************************************************
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

class ProfileUtils
{
public:
    ProfileUtils(const char* szFilePath);
    ~ProfileUtils(void);

    /* 
    ** 向配置文件写入信息
    ** @param szSectionName: 区块名 
    ** @param szKey: 键
    ** @param szValue: 值 
    ** @return 操作成功返回true, 失败返回false
    */
    bool Write2Profile(const char* szSectionName, const char* szKey, const char* szValue);

    /* 
    ** 从配置文件读取信息
    ** @param szSectionName: 区块名
    ** @param szKey: 键
    ** @param szDefault: 默认值
    ** @param pValueBuff: 接收值的缓冲区
    ** @param nBufSize: 缓冲区大小
    ** @return 操作成功返回true, 失败返回false
    */
    bool GetProfile(const char* szSectionName, const char* szKey, const char* szDefault, char* pValueBuff, size_t nBufSize); 

private:
    char m_szFilePath[MAX_PATH];
    bool m_bInited;
    
};

