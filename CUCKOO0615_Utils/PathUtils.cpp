
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�·������
//*************************************************

#include "PathUtils.h"
#include <direct.h>
#include <assert.h>

#define GetStringLength(szString)     (szString ? ::strlen(szString) : 0)

#ifdef CUCKOO0615_USE_STL
bool PathUtils::GetFilesInDir(std::vector<std::string>& vecFileFullPaths, 
    std::string strRootDir, std::string strzWildcard, 
    bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes)
{
    return GetFullPathsInDir(vecFileFullPaths, strRootDir, strzWildcard, bEnterSubDir, bEnterHiddenSubDir, nExceptFileTypes, true, false);
}

bool PathUtils::GetSubDirsInDir(std::vector<std::string>& vecSubDirFullPaths, 
    std::string strRootDir, std::string strzWildcard, bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes)
{
    return GetFullPathsInDir(vecSubDirFullPaths, strRootDir, strzWildcard, bEnterSubDir, bEnterHiddenSubDir, nExceptFileTypes, false, true);
}

bool PathUtils::GetFullPathsInDir(std::vector<std::string>& vecFullPaths, 
    std::string strRootDir, const std::string& strzWildcard, bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes, bool bGetFiles, bool bGetDirs)
{
    if (strRootDir.length())
    {
        char chLastChar = *(strRootDir.end() - 1);
        if ('\\' != chLastChar && '/' != chLastChar)
            strRootDir += "\\";
    }
    std::string strSearchPath = strRootDir + strzWildcard;

    intptr_t handle = NULL;
    _finddata_t fileinfo;
    handle = _findfirst(strSearchPath.c_str(), &fileinfo);
    if (-1 == handle)
    {
        _findclose(handle);
        return false;
    }

    do
    {
        if (!::strcmp(fileinfo.name, ".") || !::strcmp(fileinfo.name, ".."))
            continue;

        bool bIsDir = !(0 == (fileinfo.attrib & _A_SUBDIR));
        bool bIsFile = !bIsDir;
        bool bAtrribIsQualified = true;
        if (-1 != nExceptFileTypes)
            bAtrribIsQualified = !(fileinfo.attrib & nExceptFileTypes);

        if (bGetFiles && bIsFile && bAtrribIsQualified)
            vecFullPaths.push_back(strRootDir + std::string(fileinfo.name));

        if (bGetDirs && bIsDir && bAtrribIsQualified)
            vecFullPaths.push_back(strRootDir + std::string(fileinfo.name));

        if (bEnterSubDir && bIsDir)
        {
            if ((fileinfo.attrib & _A_HIDDEN) && !bEnterHiddenSubDir)
                continue;

            std::string strSubPath = strRootDir + fileinfo.name;
            if (!GetFullPathsInDir(
                vecFullPaths, strSubPath.c_str(), strzWildcard.c_str(), 
                true, bEnterHiddenSubDir, nExceptFileTypes, bGetFiles, bGetDirs))
            {
                _findclose(handle);
                return false;
            }
        }
    } while (!_findnext(handle, &fileinfo));

    _findclose(handle);
    return true;
}

std::string PathUtils::GetFileName(const std::string& strFileFullPath)
{        
    if (!strFileFullPath.length())
        return "";

    using namespace std;
    string::const_iterator itBeg = strFileFullPath.begin();
    string::const_iterator itSearch = strFileFullPath.end() - 1;
    for (; itSearch != itBeg; --itSearch)
        if ('\\' == *itSearch || '/' == *itSearch)
            return string(itSearch + 1, strFileFullPath.end());
    return "";
}

std::string PathUtils::GetParentPath(const std::string& strPath)
{
    if (strPath.length() <= 1)
        return "";

    using namespace std;
    string::const_iterator itBeg = strPath.begin();
    string::const_iterator itSearch = strPath.end() - 1;
    if ('\\' == *itSearch || '/' == *itSearch)
        --itSearch;
    for (; itSearch != itBeg; --itSearch)
        if ('\\' == *itSearch || '/' == *itSearch)
            return string(itBeg, itSearch);
    return "";
}

#endif

bool PathUtils::GetFileName(const char* szFullPath, char* szName, size_t nNameBufSize)
{
    size_t nFullPathLength = GetStringLength(szFullPath);
    if (!nFullPathLength)
    {
        ::memset(szName, 0x00, nNameBufSize);
        return true;
    }
    if (nFullPathLength > MAX_PATH) nFullPathLength = MAX_PATH;

    char szBuf[MAX_PATH] = { 0 };
    ::memcpy(szBuf, szFullPath, nFullPathLength);
    szBuf[MAX_PATH - 1] = 0x00;

    size_t nPos = strlen(szBuf) - 1;
    if ('/' == szBuf[nPos] || '\\' == szBuf[nPos])
        szBuf[nPos] = 0x00;
    for (; nPos != -1; --nPos)
    {
        if ('/' == szBuf[nPos] || '\\' == szBuf[nPos])
        {
            size_t nOffset = nPos + 1;
            ::memcpy(szName, szBuf + nOffset, strlen(szBuf) - nOffset);
            break;
        }
    }
    return (-1 != nPos);
}

int PathUtils::GetDriveNames(char arrLogicalDriveNames[26])
{
    const int nBufSize = 26 * 4;
    char szBuf[nBufSize] = { 0 };
    DWORD dwLength = ::GetLogicalDriveStringsA(nBufSize, szBuf);

    if ((0 == dwLength) || (nBufSize <= dwLength))
        return -1;

    int i = 0;
    for (char * s = szBuf; *s; s += (strlen(s) + 1))
    {
        arrLogicalDriveNames[i] = s[0];
        ++i;
    }
    return i;
}

const char* PathUtils::GetDriveType(char chDriveName)
{
    char szDriveName[] = "X:\\";
    szDriveName[0] = chDriveName;
    UINT uDriveType = ::GetDriveTypeA(szDriveName);

    switch (uDriveType)
    {
    case DRIVE_UNKNOWN:
        return "δ֪������";
    case DRIVE_NO_ROOT_DIR:
        return "��Ч��Ŀ¼";
    case DRIVE_REMOVABLE:
        return "���ƶ��豸";
    case DRIVE_FIXED:
        return "�̶�������";
    case DRIVE_REMOTE:
        return "����������";
    case DRIVE_CDROM:
        return "CD-ROM";
    case DRIVE_RAMDISK:
        return "RAM������";
    default:
        return "δ֪�豸";
    }
}

const char* PathUtils::GetAppFullPath()
{
    static char buffRet[MAX_PATH] = { 0 };
    DWORD dwRet = ::GetModuleFileNameA(NULL, buffRet, MAX_PATH);

    if (0 == dwRet)
        return "";
    if (MAX_PATH == dwRet)
    {
        static char buffBackup[4096] = { 0 };
        ::GetModuleFileNameA(NULL, buffBackup, 4096);
        return buffBackup;
    }
    return buffRet;
}

const char* PathUtils::GetAppDirectory()
{
    static char buffRet[MAX_PATH] = { 0 };
    char* pBuff = buffRet;
    DWORD dwRet = ::GetModuleFileNameA(NULL, pBuff, MAX_PATH);

    if (0 == dwRet)
        return "";
    if (MAX_PATH == dwRet)
    {
        static char buffBackup[4096] = { 0 };
        pBuff = buffBackup;
        dwRet = ::GetModuleFileNameA(NULL, pBuff, 4096);
    }
    while ('\\' != pBuff[dwRet] && dwRet)
        --dwRet;
    pBuff[dwRet] = '\0';
    return pBuff;
}

bool PathUtils::PathIsExist(const char* szPath)
{
    if (0 == GetStringLength(szPath))
        return false;
    return (0 == _access(szPath, 0));
}

bool PathUtils::FileIsExist(const char* szFilePath)
{
    if(0 == GetStringLength(szFilePath))
        return false;
   
    _finddata_t fileinfo;

#if _MSC_VER >= 1400	// intptr_t/long
    intptr_t handle = _findfirst(szFilePath, &fileinfo);
#else
    long handle = _findfirst( szFilePath, &fileinfo);
#endif

    bool bIsDir = (0 != (fileinfo.attrib & _A_SUBDIR));
    bool ret = true;
    if ((-1 == handle) || bIsDir)
        ret = false;

    _findclose(handle);
    return ret;
}

bool PathUtils::DirIsExist(const char* szDirPath)
{
    if (0 == GetStringLength(szDirPath))
        return false;

    _finddata_t fileinfo;

#if _MSC_VER >= 1400	// intptr_t/long
    intptr_t handle = _findfirst(szDirPath, &fileinfo);
#else
    long handle = _findfirst( szDirPath, &fileinfo);
#endif

    bool bIsDir = (0 != (fileinfo.attrib & _A_SUBDIR));
    bool ret = true;
    if ((-1 == handle) || !bIsDir)
        ret = false;

    _findclose(handle);
    return ret;
}

bool PathUtils::GetParentPath(const char* szPath, char* szParentPath, size_t nParentPathBufSize)
{
    size_t nStrLength = GetStringLength(szPath);
    if (!nStrLength)
    {
        ::memset(szParentPath, 0x00, nParentPathBufSize);
        return true;
    }
    if (nStrLength > MAX_PATH) nStrLength = MAX_PATH;

    char szBuf[MAX_PATH] = { 0 };
    ::memcpy(szBuf, szPath, nStrLength);
    szBuf[MAX_PATH - 1] = 0x00;

    size_t nPos = ::strlen(szBuf) - 1;
    if ('\\' == szBuf[nPos] || '/' == szBuf[nPos])
        szBuf[nPos] = 0x00;

    for (; nPos != -1; --nPos)
    {
        if ('\\' == szBuf[nPos] || '/' == szBuf[nPos])
        {
            if (nPos >= nParentPathBufSize)
                return false;
            ::memcpy(szParentPath, szBuf, nPos);
            break;
        }
    }
    return (-1 != nPos);
}

bool PathUtils::CreateMultiDirectory(const char* szPath)
{
    size_t nStrLength = GetStringLength(szPath);
    if (0 == nStrLength)
        return true;
    if (nStrLength > MAX_PATH) 
        nStrLength = MAX_PATH;

    char szTmp[MAX_PATH] = { 0 };
    for (size_t i = 0; i != nStrLength + 1; ++i)
    {
        if ('/' == szPath[i] || '\\' == szPath[i] || 0x00 == szPath[i])
        {
            ::memcpy(szTmp, szPath, i);
            if (!::_access(szTmp, 0)) 
                continue;
            if (::_mkdir(szTmp))       
                return false;
        }
    }
    return true;
}

void PathUtils::FixBackSlashInPath(std::string& strPath)
{
    std::string::size_type nStrLength = strPath.length();
    if (!nStrLength)
        return;
    for (std::string::size_type i = 0; i != nStrLength; ++i)
    {
        if ('/' == strPath[i])
            strPath[i] = '\\';
    }
    for (std::string::size_type nPos = strPath.find("\\\\");
        std::string::npos != nPos;
        nPos = strPath.find("\\\\"))
    {
        strPath.replace(nPos, 2, "\\");
    }
}

void PathUtils::FixBackSlashInFilePath(std::string& strFilePath)
{
    if (!strFilePath.length())
        return;
    FixBackSlashInPath(strFilePath);
    if ('\\' == *(strFilePath.end() - 1))
        strFilePath.erase(strFilePath.end() - 1);
}

void PathUtils::FixBackSlashInDirPath(std::string& strDirPath)
{
    if (!strDirPath.length())
        return;
    FixBackSlashInPath(strDirPath);
    if ('\\' != *(strDirPath.end() - 1))
        strDirPath += "\\";
}

#ifdef __AFXWIN_H__

void PathUtils::FixBackSlash_DirPath(CString& strDirPath)
{
    int nLength = strDirPath.GetLength();
    if (0 == nLength)
        return;
    strDirPath.Replace(_T('/'), _T('\\'));
    strDirPath += _T('\\');
    while (0 != strDirPath.Replace(_T("\\\\"), _T("\\")));
}

void PathUtils::FixBackSlash_FilePath(CString& strFilePath)
{
    FixBackSlash_DirPath(strFilePath);
    if (0 != strFilePath.GetLength())
        strFilePath.TrimRight(_T('\\'));
}

void PathUtils::FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath)
{
    int nLength = strFtpRemotePath.GetLength();
    if (0 == nLength)
        return;
    strFtpRemotePath.Replace(_T('\\'), _T('/'));
    strFtpRemotePath += _T('/');
    while (0 != strFtpRemotePath.Replace(_T("//"), _T("/")));
}

void PathUtils::FixSlash_FtpRemoteFilePath(CString& strFtpRemotePath)
{
    FixSlash_FtpRemoteDirPath(strFtpRemotePath);
    if (0 != strFtpRemotePath.GetLength())
        strFtpRemotePath.TrimRight(_T('/'));
}
#endif

