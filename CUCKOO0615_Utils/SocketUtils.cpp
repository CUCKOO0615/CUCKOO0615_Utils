
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月14日
//◇说明：Socket工具
//*************************************************

#include "StdAfx.h"
#include "SocketUtils.h"

bool SocketUtils::m_bIsInited = false;
char SocketUtils::m_szInitFailedErrMsg[ERRMSG_LENGTH] = { 0 };

SocketUtils::SocketUtils(void)
{
    memset(m_szErrMsg, 0x00, ERRMSG_LENGTH);
    memset(m_szInitFailedErrMsg, 0x00, ERRMSG_LENGTH);
}

SocketUtils::~SocketUtils(void)
{
}

bool SocketUtils::InitSocketUtils()
{
    if (m_bIsInited)
        return true;

    WORD wVersionRequested = MAKEWORD(1, 1);
    WSADATA wsaData;

    int nRet = WSAStartup(wVersionRequested, &wsaData);
    if (0 == nRet)
    {
        m_bIsInited = true;
        memset(m_szInitFailedErrMsg, 0, ERRMSG_LENGTH);
        return true;
    }
    else
    {
        strcpy(m_szInitFailedErrMsg, "Socket init failed, ");
		strcat(m_szInitFailedErrMsg, SocketUtils::QueryErrMsg(nRet));
        return false;
    }

}

SOCKET SocketUtils::CreateClientSocket_TCP(const char* szServerAddr, u_short usPort)
{
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return INVALID_SOCKET;
    }

    SOCKET socketRet = socket(AF_INET, SOCK_STREAM, 0);

    if (INVALID_SOCKET == socketRet)
    {
        SetErrMsg("Create client socket failed, ");
		int nErrCode = WSAGetLastError();
		strcat(m_szErrMsg, SocketUtils::QueryErrMsg(nErrCode));
        return INVALID_SOCKET;
    }

    SOCKADDR_IN addrClient;
    addrClient.sin_family = AF_INET;
    addrClient.sin_addr.S_un.S_addr = inet_addr(szServerAddr);
    addrClient.sin_port = htons(usPort);

    int nRet = connect(socketRet, (SOCKADDR*)&addrClient, sizeof(addrClient));
    if (SOCKET_ERROR == nRet)
    {
        SetErrMsg("Create client socket failed, ");
		int nErrCode = WSAGetLastError();
		strcat(m_szErrMsg, SocketUtils::QueryErrMsg(nErrCode));
		closesocket(socketRet);
        return INVALID_SOCKET;
    }
    return socketRet;
}

bool SocketUtils::ReleaseSocketUtils()
{
    m_bIsInited = !(0 == WSACleanup()) || (WSANOTINITIALISED == WSACleanup());
    return !m_bIsInited;
}

SOCKET SocketUtils::CreateServerSocket_TCP(u_short usPort /*= 10086 */)
{
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return INVALID_SOCKET;
    }

	SOCKET socketRet = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socketRet)
    {
        SetErrMsg("Create server socket failed, ");
		int nErrCode = WSAGetLastError();
		strcat(m_szErrMsg, SocketUtils::QueryErrMsg(nErrCode));
        return INVALID_SOCKET;
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(usPort);

	int nRet = bind(socketRet, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
    if (SOCKET_ERROR == nRet)
    {
        SetErrMsg("Bind server socket failed, ");
		int nErrCode = WSAGetLastError();
		strcat(m_szErrMsg, SocketUtils::QueryErrMsg(nErrCode));
		closesocket(socketRet);
        return INVALID_SOCKET;
    }
	return socketRet;
}

bool SocketUtils::ListenAt(SOCKET skSrvSock, int nBackLog)
{
	int nRet = ::listen(skSrvSock, nBackLog);
	if (0 != nRet)
	{
		SetErrMsg("Listen at specified socket failed, ");
		int nErrCode = WSAGetLastError();
		strcat(m_szErrMsg, SocketUtils::QueryErrMsg(nErrCode));
		return false;
	}
	return true;
}

const char* SocketUtils::QueryErrMsg(size_t nErrCode)
{
	for (int i = 0; i != ERRMSGTABLE_LENGTH; ++i)
	{
		if (ErrMsgTable[i].lRetVal == nErrCode)
			return ErrMsgTable[i].szErrMsg;
	}
	return "UNKNOWERROR";
}

bool SocketUtils::SendToSocket(SOCKET s, char* pBuffer, int nSpecLength)
{
	int nRemainingChars = nSpecLength;
	while (0 < nRemainingChars)
	{
		int nSendedChars = ::send(s, pBuffer + nSpecLength - nRemainingChars, nRemainingChars, NULL);
		if (SOCKET_ERROR == nSendedChars)
		{
			SetErrMsg("Socket returned SOCKET_ERROR");
			return false;
		}
		nRemainingChars -= nSendedChars;
	}
	return true;
}

bool SocketUtils::RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength)
{
	int nRecvedLen = 0;
	while (nRecvedLen < nSpecLength)
	{
		int nRecv = ::recv(s, pBuffer + nRecvedLen, nSpecLength - nRecvedLen, NULL);
		if (SOCKET_ERROR == nRecv)
		{
			SetErrMsg("Socket returned SOCKET_ERROR");
			return false;
		}
		if (0 == nRecv)	           
			continue;
		nRecvedLen += nRecv;
	}
	return true;
}