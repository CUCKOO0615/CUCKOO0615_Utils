
#pragma once
#include <windows.h>

namespace WinAPI_Utils
{
    /*
    ** ����һ��Ĭ��Ȩ�޼���(���ܱ��ӽ��̼̳�)�Ļ���������
    ** @param bInitialOwner: �������������Ƿ��ɱ��߳������������Ȩ, TRUE���, FALSE����
    ** @param szName: ����������������,Ĭ��Ϊ����
    ** @return: �ɹ�������Ч�Ļ�����������, ʧ�ܷ���NULL, ��GetLastError��ȡ������
    */
    HANDLE CreateMutex(BOOL bInitialOwner, LPCTSTR szName = NULL);

    /*
    ** �ͷ�ָ�����������������Ȩ
    ** @param hMutex: ������������
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false, ��GetLastError��ȡ������
    */
    bool ReleaseMutex(HANDLE hMutex);

    /*
    ** ���ָ�����ֵĻ����������Ƿ����
    ** @param szName: ָ���Ļ�����������
    ** @return: �������Ѵ��ڷ���true, ���򷵻�false
    */
    bool MutexIsExist(LPCTSTR szName);

    //////////////////////////////////////////////////////////////////////////

    /*
    ** ��ȡGetLastError()���صĴ����������Ĵ�����Ϣ
    ** @Param dwErrCode: ��GetLastError()���صĴ����
    ** @Param szErrMsg: ָ�������Ϣ�ַ�����ָ��,����Ҫ����ռ�(new)
    ** @Ret: ��ϵͳ������������ɴ�����Ϣ���ڴ滺�������
    ** - PS: �þ����Ҫ��FreeLastErrMsgBuffer�����ֶ��ͷ�,��������ڴ�й©.
    ** -   : �����ɹ�,szErrMsgָ�������Ϣ�ַ���; ����ʧ��,szErrMsgָ��NULL.
    */
    HLOCAL GetLastErrMsg(DWORD dwErrCode, TCHAR*& szErrMsg);

    /*
    ** �ͷ�GetLastErrMsg�������ص��ڴ���
    ** @Param hLocal: �ڴ���
    */
    void FreeLastErrMsgBuffer(HLOCAL& hLocal);

};

