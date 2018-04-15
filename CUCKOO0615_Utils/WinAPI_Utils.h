
#pragma once
#include <windows.h>

namespace WinAPI_Utils
{
    namespace MappingFileUtils
    {
        enum MappingType
        {
            MT_ReadOnly,
            MT_ReadWrite
        };
        struct MappingFileInfo;

        /*
        ** �����ڴ�ӳ���ļ�
        ** @Param szFilePath: �ļ�·��
        ** @Param emMT: �ļ���д���� MT_ReadOnly/MT_ReadWrite
        ** @Param mfi: �ļ���Ϣ�ṹ��, ��ԱpbFile��Ϊ��дָ��, mfi����ʱ���Զ��ͷ���Դ
        ** @Param dwFileSize: �������ļ�ʱ,��ָ��һ����0���ļ���С, ���������ļ�ʱ�ò���������
        ** @Ret : �����ɹ�����0, �ڲ�API����ʧ�ܺ�, DWORD GetLastError()�ķ���ֵ
        */
        DWORD CreateMappingFile(const TCHAR* szFilePath, MappingType emMT, MappingFileInfo& mfi, DWORD dwFileSize = 0);


    }

    namespace MutexUtils
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
    }

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

    //////////////////////////////////////////////////////////////////////////
    struct MappingFileUtils::MappingFileInfo
    {
        HANDLE hFileHandle;
        HANDLE hFileMappingHandle;
        LPBYTE pbFile;

        MappingFileInfo() : hFileHandle(NULL), hFileMappingHandle(NULL), pbFile(NULL){}
        ~MappingFileInfo()
        {
            if (hFileHandle)
                ::CloseHandle(hFileHandle);
            if (hFileMappingHandle)
                ::CloseHandle(hFileMappingHandle);
            if (pbFile)
                ::UnmapViewOfFile(pbFile);
        }
    };
};

