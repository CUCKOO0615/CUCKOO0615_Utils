//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014/11/19 16:05:53
//��˵���������ļ�����
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
    ** �������ļ�д����Ϣ
    ** @param szSectionName: ������ 
    ** @param szKey: ��
    ** @param szValue: ֵ 
    ** @return �����ɹ�����true, ʧ�ܷ���false
    */
    bool Write2Profile(const char* szSectionName, const char* szKey, const char* szValue);

    /* 
    ** �������ļ���ȡ��Ϣ
    ** @param szSectionName: ������
    ** @param szKey: ��
    ** @param szDefault: Ĭ��ֵ
    ** @param pValueBuff: ����ֵ�Ļ�����
    ** @param nBufSize: ��������С
    ** @return �����ɹ�����true, ʧ�ܷ���false
    */
    bool GetProfile(const char* szSectionName, const char* szKey, const char* szDefault, char* pValueBuff, size_t nBufSize); 

private:
    char m_szFilePath[MAX_PATH];
    bool m_bInited;
    
};

