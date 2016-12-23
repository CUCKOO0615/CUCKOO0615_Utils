#include "StdAfx.h"
#include "SingleSocket.h"
#include <map>

#pragma region Error Message
#ifndef ERRMSGTABLE_LENGTH
#define ERRMSGTABLE_LENGTH 41
#endif

typedef struct
{
    long lRetVal;
    const char* szErrMsg;
}ERRMSG_TABLE;

static std::map<int, const char*> ErrMsgMap;
static void InitErrMsgMap()
{
    //Init
    ErrMsgMap[WSASYSNOTREADY] = "WSASYSNOTREADY";
    ErrMsgMap[WSAVERNOTSUPPORTED] = "WSAVERNOTSUPPORTED";
    ErrMsgMap[WSAEINPROGRESS] = "WSAEINPROGRESS";
    ErrMsgMap[WSAEPROCLIM] = "WSAEPROCLIM";
    ErrMsgMap[WSAEFAULT] = "WSAEFAULT";

    //socket
    ErrMsgMap[WSANOTINITIALISED] = "WSANOTINITIALISED";
    ErrMsgMap[WSAENETDOWN] = "WSAENETDOWN";
    ErrMsgMap[WSAEAFNOSUPPORT] = "WSAEAFNOSUPPORT";
    ErrMsgMap[WSAEINPROGRESS] = "WSAEINPROGRESS";
    ErrMsgMap[WSAEMFILE] = "WSAEMFILE";
    ErrMsgMap[WSAENOBUFS] = "WSAENOBUFS";
    ErrMsgMap[WSAEPROTONOSUPPORT] = "WSAEPROTONOSUPPORT";
    ErrMsgMap[WSAEPROTOTYPE] = "WSAEPROTOTYPE";
    ErrMsgMap[WSAESOCKTNOSUPPORT] = "WSAESOCKTNOSUPPORT";

    //connect
    ErrMsgMap[WSANOTINITIALISED] = "WSANOTINITIALISED";
    ErrMsgMap[WSAENETDOWN] = "WSAENETDOWN";
    ErrMsgMap[WSAEADDRINUSE] = "WSAEADDRINUSE";
    ErrMsgMap[WSAEINTR] = "WSAEINTR";
    ErrMsgMap[WSAEINPROGRESS] = "WSAEINPROGRESS";
    ErrMsgMap[WSAEADDRNOTAVAIL] = "WSAEADDRNOTAVAIL";
    ErrMsgMap[WSAEAFNOSUPPORT] = "WSAEAFNOSUPPORT";
    ErrMsgMap[WSAECONNREFUSED] = "WSAECONNREFUSED";
    ErrMsgMap[WSAEDESTADDRREQ] = "WSAEDESTADDRREQ";
    ErrMsgMap[WSAEFAULT] = "WSAEFAULT";
    ErrMsgMap[WSAEINVAL] = "WSAEINVAL";
    ErrMsgMap[WSAEISCONN] = "WSAEISCONN";
    ErrMsgMap[WSAEMFILE] = "WSAEMFILE";
    ErrMsgMap[WSAENETUNREACH] = "WSAENETUNREACH";
    ErrMsgMap[WSAENOBUFS] = "WSAENOBUFS";
    ErrMsgMap[WSAENOTSOCK] = "WSAENOTSOCK";
    ErrMsgMap[WSAETIMEDOUT] = "WSAETIMEDOUT";
    ErrMsgMap[WSAEWOULDBLOCK] = "WSAEWOULDBLOCK";

    //bind
    ErrMsgMap[WSANOTINITIALISED] = "WSANOTINITIALISED";
    ErrMsgMap[WSAENETDOWN] = "WSAENETDOWN";
    ErrMsgMap[WSAEADDRINUSE] = "WSAEADDRINUSE";
    ErrMsgMap[WSAEFAULT] = "WSAEFAULT";
    ErrMsgMap[WSAEINPROGRESS] = "WSAEINPROGRESS";
    ErrMsgMap[WSAEAFNOSUPPORT] = "WSAEAFNOSUPPORT";
    ErrMsgMap[WSAEINVAL] = "WSAEINVAL";
    ErrMsgMap[WSAENOBUFS] = "WSAENOBUFS";
    ErrMsgMap[WSAENOTSOCK] = "WSAENOTSOCK";
}

const char* SingleSocket::QueryErrMsg(size_t nErrCode)
{
    if (ErrMsgMap.end() != ErrMsgMap.find(nErrCode))
        return ErrMsgMap[nErrCode];   
    return "UNKNOWERROR";
}

#define RESET_ERRMSG   if (m_bIsInited) m_szErrMsg[0] = '\0'
#pragma endregion

SingleSocket::SingleSocket() :
m_bIsInited(false),
m_sSingleSocket(INVALID_SOCKET),
m_emSocketType(SS_NotSet)
{
    InitErrMsgMap();
}

SingleSocket::~SingleSocket()
{
    if (m_bIsInited)
        ::WSACleanup();
}

bool SingleSocket::Init()
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        WORD wVersionRequested = MAKEWORD(1, 1);
        WSADATA wsaData;

        int nRet = ::WSAStartup(wVersionRequested, &wsaData);
        if (0 == nRet)
            m_bIsInited = true;
        else
            FmtErrMsg("Socket init failed, %s", QueryErrMsg(nRet));
    }
    return m_bIsInited;
}

bool SingleSocket::CreateSocket(SingleSocketType emType, const char* szIpAddr, unsigned short usPort)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (emType == SS_NotSet)
    {
        SetErrMsg("SingleSocketType cannot be SS_NotSet");
        return false;
    }

    SOCKET socketRet = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == socketRet)
    {
        int nErrCode = WSAGetLastError();
        FmtErrMsg("Create socket failed, %s", QueryErrMsg(nErrCode));
        return false;
    }

    SOCKADDR_IN sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(usPort);
    if (SS_Client == emType)
    {
        sockAddr.sin_addr.S_un.S_addr = inet_addr(szIpAddr);
        int nRet = connect(socketRet, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        if (SOCKET_ERROR == nRet)
        {
            int nErrCode = WSAGetLastError();
            FmtErrMsg("Create client connect failed, %s",QueryErrMsg(nErrCode));
            closesocket(socketRet);
            return false;
        }
        m_emSocketType = SS_Client;
    }
    else
    {
        sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        int nRet = bind(socketRet, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        if (SOCKET_ERROR == nRet)
        {
            int nErrCode = WSAGetLastError();
            FmtErrMsg("Bind server socket failed, %s",QueryErrMsg(nErrCode));
            closesocket(socketRet);
            return false;
        }
        m_emSocketType = SS_Server;
    }
    m_sSingleSocket = socketRet;
    return true;
}

bool SingleSocket::StartListening(int nBackLog)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (SS_Server != m_emSocketType)
    {
        SetErrMsg("Socket type is not SS_Server");
        return false;
    }

    int nRet = ::listen(m_sSingleSocket, nBackLog);
    if (0 != nRet)
    {
        int nErrCode = WSAGetLastError();
        FmtErrMsg("Listen at specified socket failed, %s",QueryErrMsg(nErrCode));
        return false;
    }
    return true;
}

bool SingleSocket::SendToServer(char* pBuffer, int nLength)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (SS_Client != m_emSocketType)
    {
        SetErrMsg("Socket type is not SS_Client");
        return false;
    }

    int nRemainingChars = nLength;
    while (0 < nRemainingChars)
    {
        int nSendedChars = ::send(
            m_sSingleSocket, 
            pBuffer + nLength - nRemainingChars, 
            nRemainingChars, 
            NULL);
        if (SOCKET_ERROR == nSendedChars)
        {
            int nErrCode = WSAGetLastError();
            FmtErrMsg("Returned SOCKET_ERROR, %s", QueryErrMsg(nErrCode));
            return false;
        }
        nRemainingChars -= nSendedChars;
    }
    return true;
}

bool SingleSocket::RecvFromServer(char* pBuffer, int nLength)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (SS_Client != m_emSocketType)
    {
        SetErrMsg("Socket type is not SS_Client");
        return false;
    }

    int nRecvedLen = 0;
    while (nRecvedLen < nLength)
    {
        int nRecv = ::recv(
            m_sSingleSocket, 
            pBuffer + nRecvedLen, 
            nLength - nRecvedLen, 
            NULL);
        if (SOCKET_ERROR == nRecv)
        {
            int nErrCode = WSAGetLastError();
            FmtErrMsg("Returned SOCKET_ERROR, %s", QueryErrMsg(nErrCode));
            return false;
        }
        if (0 == nRecv)
            continue;
        nRecvedLen += nRecv;
    }
    return true;
}

void SingleSocket::FmtErrMsg(const char* szFormat, ...)
{
    va_list argList;
    va_start(argList, szFormat);
    ::vsprintf(m_szErrMsg, szFormat, argList);
    va_end(argList);
}

void SingleSocket::Reset()
{
    if (!m_bIsInited)
        return;

    ::closesocket(m_sSingleSocket);
    m_sSingleSocket = INVALID_SOCKET;
    RESET_ERRMSG;
}

bool SingleSocket::SetSocketTimeOut(int nMilliseconds)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (nMilliseconds <= 0) 
        return true;
    if (SS_Server != m_emSocketType && SS_Client != m_emSocketType)
    {
        SetErrMsg("Socket type is not set");
        return false;
    }

    if (SS_Client == m_emSocketType)
    {
        if (!::setsockopt(m_sSingleSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&nMilliseconds, sizeof(int)))
            return true;
    }
    else if (SS_Server == m_emSocketType)
    {
        if (!::setsockopt(m_sSingleSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nMilliseconds, sizeof(int)))
            return true;
    }
    int nErrCode = WSAGetLastError();
    FmtErrMsg("Returned SOCKET_ERROR, %s", QueryErrMsg(nErrCode));
    return false;
}

bool SingleSocket::GetAddressBySocket(SOCKADDR_IN & addr)
{
    RESET_ERRMSG;
    if (!m_bIsInited)
    {
        SetErrMsg("Socket has not been initialized");
        return false;
    }
    if (SS_Client != m_emSocketType)
    {
        SetErrMsg("Socket type is not SS_Client");
        return false;
    }

    ::memset(&addr, 0, sizeof(SOCKADDR_IN));
    int nAddrLen = sizeof(SOCKADDR_IN);

    //根据套接字获取地址信息
    if (::getpeername(m_sSingleSocket, (SOCKADDR*)&addr, &nAddrLen))
    {
        int nErrCode = ::WSAGetLastError();
        SetErrMsg(QueryErrMsg(nErrCode));
        return false;
    }
    return true;
}

bool SingleSocket::SendToSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode)
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

bool SingleSocket::RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode)
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

bool SingleSocket::GetAddressBySocket(SOCKET s, SOCKADDR_IN & addr, int& nErrCode)
{
    nErrCode = 0;
    ::memset(&addr, 0, sizeof(SOCKADDR_IN));
    int nAddrLen = sizeof(SOCKADDR_IN);

    //根据套接字获取地址信息
    if (::getpeername(s, (SOCKADDR*)&addr, &nAddrLen))
    {
        nErrCode = ::WSAGetLastError();
        return false;
    }
    return true;
}

