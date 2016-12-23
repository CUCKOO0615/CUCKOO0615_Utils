//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014/10/15 15:31:20
//��˵����������ز���
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

#include <vector>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#pragma comment(lib,"psapi.lib")

class ProcessUtils
{
public:
    /*
    ** ��ȡ��ǰ��������������н���PROCESSENTRY32�ṹ����б�
    ** @param vecPsEntry: �����ɹ�����PROCESSENTRY32�ṹ���б�
    ** @return: �����ɹ�����true, ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    static bool GetProcessEntryList(std::vector<PROCESSENTRY32>& vecPsEntry);

    /*
    ** ��ȡָ��PROCESSENTRY32�ṹ���еĽ���ID��Ϣ
    ** @param psEntry: ����������Ϣ��PROCESSENTRY32�ṹ��
    */
    static DWORD GetProcessID(const PROCESSENTRY32& psEntry);

    /*
    ** ��ֹӵ��ָ������ID�Ľ���
    ** @param dwPsID: ָ���Ľ���ID
    ** @return: �����ɹ�����true, ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    static bool KillProcess(DWORD dwPsID);

    /*
    ** ���ָ�������Ƿ����
    ** @param szPsImageName: ����ӳ����
    ** @return: �����ɹ�����true, ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    static bool CheckProcessExist(const TCHAR* szPsImageName);

    /*
    ** ��ȡָ�����̵ı����ļ�·��
    ** @param dwPsID: ָ�����̵�ID
    ** @param szNtPath: ���ؽ��̵ı����ļ�·��
    ** @return: �����ɹ�����true, ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    static bool GetProcessFilePathByID(DWORD dwPsID, TCHAR* szNtPath);

    /*
    ** ��ȡָ���豸Ŀ¼���߼�Ŀ¼
    ** @param pszDosPath: �豸Ŀ¼
    ** @param pszNtPath: �߼�Ŀ¼�ַ���������ָ��, �豣���㹻�Ĵ�С
    ** @return: �����ɹ�����true, pszNtPathָ��Ļ������а���ת������߼�Ŀ¼
    ** @return: ����ʧ�ܷ���false, ��GetErrMsg��ȡ������Ϣ
    */
    static bool DosPath2NtPath(const TCHAR* pszDosPath, TCHAR* pszNtPath);

    // ������Ϣ
    static const char* GetLastErrMsg() { return _szErrMsg; }

private:
    static char _szErrMsg[ERRMSG_LENGTH];
};

