
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�����
//*************************************************
#pragma once

#ifndef OUT
#define OUT
#endif

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

namespace FileUtils
{
    /*
    ** ��ȡ�ļ��ֽڴ�С
    ** @Param szFilePath: �ļ�·��
    ** @Param nFileSize: �ļ��ֽڴ�С
    ** @Ret : �����ɹ�����0������ʧ�ܷ�����GetLastError()��ȡ�Ĵ�����
    */
    DWORD GetFileSize(const char* szFilePath, OUT __int64 & nFileSize);

    /*
    ** ɾ���ļ�
    ** @Param szFile: �ļ�·��
    ** @Param bRecycle: true-ɾ��������վ��false-����ɾ��
    ** @Ret : �����ɹ�����true��ʧ�ܷ���false����GetLastError()��ȡ������
    */
    bool DelFile(const char* szFile, bool bRecycle);

    /*
    ** �����ļ�����޸�ʱ��
    ** @Param szFilePath: �ļ�·��
    ** @Param stuFileTime: ʱ��
    ** @Ret : �����ɹ�����0������ʧ�ܷ�����GetLastError()��ȡ�Ĵ�����
    */
    DWORD SetLastWriteTime(const char* szFilePath, const FILETIME& stuFileTime);
    DWORD SetLastWriteTime(const char* szFilePath, const SYSTEMTIME& stuSysTime);
    //DWORD SetLastWriteTime(const char* szFilePath, const time_t t);
#ifdef __AFXWIN_H__
    DWORD SetLastWriteTime(const char* szFilePath, const CTime& clsTime);
#endif

};


