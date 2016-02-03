#include "StdAfx.h"
#include "SystemUtils.h"

#include <WINSOCK2.H>
#include <windows.h>
#pragma  comment(lib,"WS2_32.lib")

bool SystemUtils::GetLocalIP(char szBuff[16])
{
	WSAData wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
		return false;

	char szHostName[MAX_PATH] = { 0 };
	if (SOCKET_ERROR == gethostname(szHostName, MAX_PATH))
	{
		WSACleanup();
		return false;
	}

	hostent* phe = gethostbyname(szHostName);
	if (NULL == phe)
	{
		WSACleanup();
		return false;
	}

	sockaddr_in addr;
	memcpy(&addr.sin_addr, phe->h_addr_list[0], sizeof(struct in_addr));
	char* szIP = inet_ntoa(addr.sin_addr);

	int nSize = min(16, strlen(szIP));
	memcpy(szBuff, szIP, nSize);

	WSACleanup();
	return true;
}
