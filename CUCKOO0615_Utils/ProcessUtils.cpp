
#include "StdAfx.h"
#include "ProcessUtils.h"

ProcessUtils::ProcessUtils(void)
{
    SetErrMsg("OK");
}

ProcessUtils::~ProcessUtils(void)
{

}

bool ProcessUtils::GetProcessEntryList(vector<PROCESSENTRY32>& vecPsEntry)
{
    HANDLE handle32Snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == handle32Snapshot)
    {
        SetErrMsg("CreateToolhelp32Snapshot failed in GetProcessEntryList");
        return false;
    }

    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(PROCESSENTRY32);
    if (TRUE == ::Process32First(handle32Snapshot, &pEntry))
    {
        vecPsEntry.push_back(pEntry);
        while (TRUE == ::Process32Next(handle32Snapshot, &pEntry))
            vecPsEntry.push_back(pEntry);
    }
    else
    {
        SetErrMsg("Process32First failed in GetProcessEntryList");
        if (0 == ::CloseHandle(handle32Snapshot))
            SetErrMsgEx(", and close handle32Snapshot failed, either");
        return false;
    }
	if (0 == :: CloseHandle(handle32Snapshot))
        SetErrMsg("Close handle32Snapshot failed in GetProcessEntryList");
    return true;
}

DWORD ProcessUtils::GetProcessID(const PROCESSENTRY32& psEntry)
{
    return psEntry.th32ProcessID;
}

bool ProcessUtils::KillProcess(DWORD dwPsID)
{
    HANDLE hl = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPsID);
    if (NULL == hl)
    {
        SetErrMsg("OpenProcess failed in KillProcess");
        return false;
    }

    bool bRet = (TRUE == TerminateProcess(hl, 0));
    if (TRUE != ::TerminateProcess(hl, 0))
    {
        SetErrMsg("Terminate process failed");
        if (0 == ::CloseHandle(hl))
            SetErrMsgEx(", and close handle failed, either.");
        return false;
    }

    if (0 == ::CloseHandle(hl))
    {
        SetErrMsg("Close handle failed in KillProcess");
        return false;
    }
    return true;
}

bool ProcessUtils::GetProcessFilePathByID(DWORD dwPsID, TCHAR* szNtPath)
{
    HANDLE hl = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPsID);
    if (NULL == hl)
    {
        SetErrMsg("OpenProcess failed in GetProcessFilePathByID");
        return false;
    }

    TCHAR szPath[MAX_PATH] = { 0 };
    if (0 == ::GetProcessImageFileName(hl, szPath, MAX_PATH))
    {
        SetErrMsg("GetProcessImageFileName failed in GetProcessFilePathByID");
        if (0 == ::CloseHandle(hl))
            SetErrMsgEx(", and close handle failed, either.");
        return false;
    }

    if (0 == ::CloseHandle(hl))
    {
        SetErrMsg("Close handle failed in GetProcessFilePathByID");
        return false;
    }
    return DosPath2NtPath(szPath, szNtPath);
}

bool ProcessUtils::DosPath2NtPath(const TCHAR* szDosPath, TCHAR* szNtPath)
{
    if (!szDosPath || !szNtPath)
    {
        SetErrMsg("Invalid parameters for DosPath2NtPath");
        return false;
    }

    TCHAR szDriveStr[27 * 4] = { 0 };
    TCHAR szDevPath[MAX_PATH] = { 0 };
    TCHAR szDrive[3] = { 0 };
    if (::GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr)) // 获取本地磁盘字符串
    {
        for (int i = 0; szDriveStr[i]; i += 4)
        {
            // 跳过A B盘
            if (!::lstrcmpi(&(szDriveStr[i]), _T("A:\\")) ||
                !::lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
                continue;

            szDrive[0] = szDriveStr[i];
            szDrive[1] = szDriveStr[i + 1];

            // 获取指定磁盘的DOS设备名
            if (!::QueryDosDevice(szDrive, szDevPath, MAX_PATH))
            {
                SetErrMsg("QueryDosDevice failed in DosPath2NtPath");
                return false;
            }

            int nStrLength = lstrlen(szDevPath);
            if (0 == _tcsnicmp(szDosPath, szDevPath, nStrLength)) // 命中  
            {
                ::lstrcpy(szNtPath, szDrive);                   // 复制驱动器 
                ::lstrcat(szNtPath, szDosPath + nStrLength);    // 复制路径 
                return true;
            }
        }
        SetErrMsg("Search driver failed in DosPath2NtPath");
    }
    else
    {
        SetErrMsg("GetLogicalDriveStrings failed in DosPath2NtPath");
    }
    return false;
}

bool ProcessUtils::CheckProcessExist(const TCHAR* szPsImageName)
{
    vector< PROCESSENTRY32 > vecPsEntrys;
    if (!GetProcessEntryList(vecPsEntrys))
        return false;

    for (size_t i = 0; i != vecPsEntrys.size(); ++i)
    {
        if (0 == ::lstrcmpi(vecPsEntrys[i].szExeFile, szPsImageName))
            return true;
    }
    return false;
}
