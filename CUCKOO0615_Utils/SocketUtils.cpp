
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月14日
//◇说明：Socket工具
//*************************************************

#include "StdAfx.h"
#include "SocketUtils.h"

bool SocketUtils::m_bIsInited = false;

bool SocketUtils::InitSocketUtils(int& nErrCode)
{
    if (m_bIsInited)
        return true;

	nErrCode = 0;
    WORD wVersionRequested = MAKEWORD(1, 1);
    WSADATA wsaData;

	nErrCode = WSAStartup(wVersionRequested, &wsaData);
	m_bIsInited = (0 == nErrCode);
	return m_bIsInited;
}

bool SocketUtils::ReleaseSocketUtils()
{
	m_bIsInited = !(0 == WSACleanup()) || (WSANOTINITIALISED == WSACleanup());
	return !m_bIsInited;
}

SOCKET SocketUtils::CreateClientSocket_TCP(int& nErrCode, const char* szServerAddr, u_short usPort)
{
	nErrCode = 0;
    SOCKET socketRet = ::socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == socketRet)
        nErrCode = ::WSAGetLastError();
    else
    {
        SOCKADDR_IN addrClient;
        addrClient.sin_family = AF_INET;
        addrClient.sin_addr.S_un.S_addr = inet_addr(szServerAddr);
        addrClient.sin_port = ::htons(usPort);

        int nRet = ::connect(socketRet, (SOCKADDR*)&addrClient, sizeof(addrClient));
        if (SOCKET_ERROR == nRet)
        {
            nErrCode = ::WSAGetLastError();
            ::closesocket(socketRet);
            return INVALID_SOCKET;
        }
    }
    return socketRet;
}

SOCKET SocketUtils::CreateServerSocket_TCP(int& nErrCode, u_short usPort /*= 10086 */)
{
	nErrCode = 0;
	SOCKET socketRet = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socketRet)
		nErrCode = ::WSAGetLastError();
    else
    {
        SOCKADDR_IN addrServer;
        addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        addrServer.sin_family = AF_INET;
        addrServer.sin_port = ::htons(usPort);

        int nRet = ::bind(socketRet, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
        if (SOCKET_ERROR == nRet)
        {
            nErrCode = ::WSAGetLastError();
            closesocket(socketRet);
            return INVALID_SOCKET;
        }
    }
	return socketRet;
}

SOCKET SocketUtils::CreateServerSocket_TCP_Ex
(int& nErrCode, u_short usPortMin, u_short usPortMax, u_short & usPort)
{
	nErrCode = 0;
	SOCKET socketRet = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == socketRet)
	{
		nErrCode = ::WSAGetLastError();
		return INVALID_SOCKET;
	}

	u_short i = usPortMin;
	for (; i <= usPortMax; ++i)
	{
		SOCKADDR_IN addrServer;
		addrServer.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
		addrServer.sin_family = AF_INET;
		addrServer.sin_port = ::htons(i);

		if (SOCKET_ERROR == ::bind(socketRet, (SOCKADDR*)&addrServer, sizeof(SOCKADDR)))
			continue;
		usPort = i;
		break;
	}
	if (i == (usPortMax + 1))
	{
		nErrCode = ::WSAGetLastError();
		::closesocket(socketRet);
		return INVALID_SOCKET;
	}
	return socketRet;
}

bool SocketUtils::ListenAt(int& nErrCode, SOCKET skSrvSock, int nBackLog)
{
	nErrCode = 0;
	int nRet = ::listen(skSrvSock, nBackLog);
	if (0 != nRet)
	{
		nErrCode = WSAGetLastError();
		return false;
	}
	return true;
}

const char* SocketUtils::QueryErrMsg(size_t nErrCode)
{
    for (int i = 0; i != ERRMSGTABLE_LENGTH; ++i)
        if (ErrMsgTable[i].lRetVal == nErrCode)
            return ErrMsgTable[i].szErrMsg;
    return "UNKNOWERROR";
}

bool SocketUtils::SendToSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode)
{
    if (nTimeOut > 0)
        ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&nTimeOut, sizeof(int));

	nErrCode = 0;
	int nRemainingChars = nSpecLength;
	while (0 < nRemainingChars)
	{
		int nSendedChars = ::send(s, pBuffer + nSpecLength - nRemainingChars, nRemainingChars, NULL);
		if (SOCKET_ERROR == nSendedChars)
		{
			nErrCode = ::WSAGetLastError();
			return false;
		}
		nRemainingChars -= nSendedChars;
	}
	return true;
}

bool SocketUtils::RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode)
{
    if (nTimeOut > 0)
        ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOut, sizeof(int));

	nErrCode = 0;
	int nRecvedLen = 0;
	while (nRecvedLen < nSpecLength)
	{
		int nRecv = ::recv(s, pBuffer + nRecvedLen, nSpecLength - nRecvedLen, NULL);
		if (SOCKET_ERROR == nRecv)
		{
			nErrCode = ::WSAGetLastError();
			return false;
		}
		if (0 == nRecv)	           
			continue;
		nRecvedLen += nRecv;
	}
	return true;
}

bool SocketUtils::GetAddressBySocket(SOCKET s, SOCKADDR_IN & addr, int& nErrCode)
{
	nErrCode = 0;
	::memset(&addr, 0, sizeof(SOCKADDR_IN));
	int nAddrLen = sizeof(SOCKADDR_IN);

	//根据套接字获取地址信息
	if (::getpeername(s, (SOCKADDR*)&addr, &nAddrLen) != 0)
	{
		nErrCode = ::WSAGetLastError();
		return false;
	}
	return true;
}


