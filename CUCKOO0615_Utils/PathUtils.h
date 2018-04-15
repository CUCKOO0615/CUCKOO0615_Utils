//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�·������
//*************************************************
#pragma once
#include "stdafx.h"

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#include <windows.h>
#include <io.h>

#define CUCKOO0615_USE_STL
#ifdef CUCKOO0615_USE_STL
#include <vector>
#include <string>
#endif

namespace PathUtils
{
    //////////////////////////////////////////////////////////////////////////
    //���� STL �ĺ���
#ifdef CUCKOO0615_USE_STL
    /*
    ** ��ȡָ���ļ����µ��������ļ���ȫ·��
    ** @param vecSubDirFullPaths: ���ļ���·������
    ** @param strRootDir: ���ļ���
    ** @param strWildcard: ͨ���
    ** @param bEnterSubDir: �Ƿ��������ļ���
    ** @param bEnterHiddenSubDir: �Ƿ������������ļ���, ����bEnterSubDirΪtrueʱ��Ч
    ** @param nExceptFileTypes: ����ʱҪ�ų����ļ�������,
    ** - ������ _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH �е�һ������,��"|"����
    */
    bool GetSubDirsInDir(
        std::vector<std::string>& vecSubDirFullPaths,
        std::string strRootDir = "",
        std::string strWildcard = "*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** ��ȡָ���ļ����µ������ļ�ȫ·��
    ** @param vecFileFullPaths: �ļ�·������
    ** @param strRootDir: ���ļ���
    ** @param strWildcard: ͨ���
    ** @param bEnterSubDir: �Ƿ��������ļ���
    ** @param bEnterHiddenSubDir: �Ƿ������������ļ���, ����bEnterSubDirΪtrueʱ��Ч
    ** @param nExceptFileTypes: ����ʱҪ�ų����ļ�����,
    ** - ������ _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH �е�һ������,��"|"����
    */
    bool GetFilesInDir(
        std::vector<std::string>& vecFileFullPaths,
        std::string strRootDir = "",
        std::string strWildcard = "*.*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** ��ȡָ���ļ����µ������ļ�ȫ·��
    ** @param vecFileFullPaths: �ļ�·������
    ** @param strRootDir: ���ļ���, �������"", ��Ĭ�ϵ�ǰĿ¼
    ** @param strWildcard: ͨ���
    ** @param bEnterSubDir: �Ƿ��������ļ���
    ** @param bEnterHiddenSubDir: �Ƿ������������ļ���, ����bEnterSubDirΪtrueʱ��Ч
    ** @param nExceptFileTypes: ����ʱҪ�ų����ļ�����,
    ** - ������ _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH �е�һ������,��"|"����
    ** @param bGetFiles: ����а����ļ�
    ** @param bGetDirs: ����а���Ŀ¼
    */
    bool GetFullPathsInDir(
        std::vector<std::string>& vecFullPaths,
        std::string strRootDir,
        const std::string& strWildcard,
        bool bEnterSubDir,
        bool bEnterHiddenSubDir,
        int nExceptFileTypes,
        bool bGetFiles,
        bool bGetDirs);

    /*
    ** ��ȫ·����ȡ�ļ������ļ�����
    ** @param strFileFullPath: �ļ����ļ��е�ȫ·��
    */
    std::string GetFileName(const std::string& strFileFullPath);
    
    /*
    ** ��ȡ��Ŀ¼·��
    ** @Param strPath: ָ����Ŀ¼·��
    ** @Ret : ��Ŀ¼·���ַ��� (ĩβ����б��)
    */
    std::string GetParentPath(const std::string& strPath);
    
    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\\'�����Ƴ�ĩβ��'\\'
    ** @Param strFilePath: �ļ�·���ַ���
    */
    void FixBackSlashInFilePath(std::string& strFilePath);

    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\\'������ĩβ����һ��'\\'
    ** @Param strDirPath: Ŀ¼·���ַ���
    */
    void FixBackSlashInDirPath(std::string& strDirPath);

    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\\'
    ** @Param strPath: Ŀ¼·���ַ���
    */
    void FixBackSlashInPath(std::string& strPath);

#endif
    
#ifdef __AFXWIN_H__
    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\'�����Һϲ������ġ�\��ֻ��һ��
    ** �������һ��\����β������ĩβ����һ��'\'
    ** @Param strDirPath: Ŀ¼·���ַ���
    */
    void FixBackSlash_DirPath(CString& strDirPath);

    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\'�����Һϲ������ġ�\��ֻ��һ��
    ** @Param strFilePath: �ļ�·���ַ���
    */
    void FixBackSlash_FilePath(CString& strFilePath);

    /*
    ** ��·���е�'\'ȫ���滻Ϊ'/'�����Һϲ������ġ�/��ֻ��һ��
    ** �������һ��/����β������ĩβ����һ��'/'
    ** @Param strFtpRemotePath: ·���ַ���
    */
    void FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath);

    /*
    ** ��·���е�'\'ȫ���滻Ϊ'/'�����Һϲ������ġ�/��ֻ��һ��
    ** @Param strFtpRemotePath: ·���ַ���
    */
    void FixSlash_FtpRemoteFilePath(CString& strFtpRemotePath);

#endif // __AFXWIN_H__

    /*
    ** ���ָ����·���Ƿ����(�������ļ����ļ���,�Ͽ�)
    ** @param szPath: Ҫ����·��
    */
    bool PathIsExist(const char* szPath);

    /*
    ** ���ָ���ļ��Ƿ����
    ** @param szFilePath: Ҫ����·��
    */
    bool FileIsExist(const char* szFilePath);

    /*
    ** ���ָ���ļ����Ƿ����
    ** @param szDirPath: Ҫ����·��
    */
    bool DirIsExist(const char* szDirPath);

    /*
    ** ��ȡ�����̷��б�
    ** @param arrLogicalDriveNames[26]: �����̷�����
    ** @return: �����ɹ������̷�����(0 - 26), ����ʧ�ܷ��� -1
    */
    int GetDriveNames(char arrLogicalDriveNames[26]);

    /*
    ** �����̷���ȡ��������
    ** @param chDriveName: �����̷�
    ** @return: δ֪������/��Ч��Ŀ¼/���ƶ��豸/�̶�������/����������/CD-ROM/RAM������/δ֪�豸
    */
    const char* GetDriveType(char chDriveName);
    
    /*
    ** ��ȡ��ǰ���̵�Ӧ�ó����ļ�·��
    ** @Ret : �ļ�·��
    */
    const char* GetAppFullPath();

    /*
    ** ��ȡ��ǰ���̵�Ӧ�ó����ļ�����Ŀ¼
    ** @Ret : Ӧ�ó����ļ�����Ŀ¼
    */
    const char* GetAppDirectory();

    /*
    ** ��ȫ·����ȡ�ļ������ļ�����
    ** @param szFullPath: �ļ����ļ��е�ȫ·��,���MAX_PATH
    ** @param szName: �ļ����ļ�����������ָ��
    ** @param nNameBufSize: �ļ����ļ�������������С
    */
    bool GetFileName(const char* szFullPath, char* szName, size_t nNameBufSize);
        
    /*
    ** ��ȡ��Ŀ¼
    ** @param szPath: �����·��,�MAX_PATH-1
    ** @param szParentPath: ��Ŀ¼������
    ** @param nParentPathBufSize: ��Ŀ¼����������,��С����Ҫ����szPath�ĳ���
    */
    bool GetParentPath(const char* szPath, char* szParentPath, size_t nParentPathBufSize);
    
    /*
    ** �����༶Ŀ¼
    ** @param szPath: ָ����Ŀ¼
    ** @return: �����ɹ�����true
    */
    bool CreateMultiDirectory(const char* szPath);

};

