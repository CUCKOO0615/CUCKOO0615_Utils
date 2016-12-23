//************************************************
//◇作者：CUCKOO0615
//◇日期：2014年8月14日
//◇说明：Socket工具
//*************************************************
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

#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef ERRMSGTABLE_LENGTH
#define ERRMSGTABLE_LENGTH 36
#endif

typedef struct
{
	long lRetVal;
	const char* szErrMsg;
}ERRMSG_TABLE;
static const ERRMSG_TABLE ErrMsgTable[ERRMSGTABLE_LENGTH] =
{
	//socket
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEMFILE, "WSAEMFILE" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT" },
	{ WSAEPROTOTYPE, "WSAEPROTOTYPE" },
	{ WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT" },

	//connect
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEADDRINUSE, "WSAEADDRINUSE" },
	{ WSAEINTR, "WSAEINTR" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAECONNREFUSED, "WSAECONNREFUSED" },
	{ WSAEDESTADDRREQ, "WSAEDESTADDRREQ" },
	{ WSAEFAULT, "WSAEFAULT" },
	{ WSAEINVAL, "WSAEINVAL" },
	{ WSAEISCONN, "WSAEISCONN" },
	{ WSAEMFILE, "WSAEMFILE" },
	{ WSAENETUNREACH, "WSAENETUNREACH" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAENOTSOCK, "WSAENOTSOCK" },
	{ WSAETIMEDOUT, "WSAETIMEDOUT" },
	{ WSAEWOULDBLOCK, "WSAEWOULDBLOCK" },

	//bind
	{ WSANOTINITIALISED, "WSANOTINITIALISED" },
	{ WSAENETDOWN, "WSAENETDOWN" },
	{ WSAEADDRINUSE, "WSAEADDRINUSE" },
	{ WSAEFAULT, "WSAEFAULT" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
	{ WSAEINVAL, "WSAEINVAL" },
	{ WSAENOBUFS, "WSAENOBUFS" },
	{ WSAENOTSOCK, "WSAENOTSOCK" }
};


class SocketUtils
{
	static bool m_bIsInited;
public:
	static const char* QueryErrMsg(size_t nErrCode);

	/*
	** 初始化WinSocket, 初始化WSA资源
	** @Param nErrCode:
	** @Ret : 操作成功返回true,失败返回false
	** @ErrCode: WSASYSNOTREADY/WSAVERNOTSUPPORTED/WSAEINPROGRESS/WSAEPROCLIM/WSAEFAULT
	*/
	static bool InitSocketUtils(int& nErrCode);

	/*
	** 释放WSA资源等
	** 调用后如需继续使用socket,必须重新调用InitSocketUtils()进行初始化
	** @Ret : 操作成功返回true,失败返回false
	*/
	static bool ReleaseSocketUtils();

	/*
	** 创建一个TCP客户端连接
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @param szServerAddr： 服务器IP地址
	** @param usPort： 服务器端口号
	** @Ret : 连接成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	static SOCKET CreateClientSocket_TCP(int& nErrCode, const char* szServerAddr = "127.0.0.1", u_short usPort = 10086);

	/*
	** 创建一个TCP服务端连接
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @param usPort： 要绑定的端口
	** @return： 建立成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	static SOCKET CreateServerSocket_TCP(int& nErrCode, u_short usPort = 10086);

	/*
	** 在指定的端口号范围内尝试建立一个TCP服务端连接
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @Param usPortMin: 最小端口号
	** @Param usPortMax: 最大端口号
	** @Param usPort: 操作成功时返回当前正在监听的端口号
	** @Ret: 建立成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	static SOCKET CreateServerSocket_TCP_Ex(int& nErrCode, u_short usPortMin, u_short usPortMax, u_short & usPort);

	/*
	** 在指定的socket上开始监听
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @Param skSrvSock: 
	** @Param nBackLog: 
	** @Ret: 操作成功返回true,失败返回false
	*/
	static bool ListenAt(int& nErrCode, SOCKET skSrvSock, int nBackLog);

	/*
	** 向指定的Socket连接发送指定的字节数
	** @Param s: 指定的Socket连接
	** @Param pBuffer: 发送缓冲区,大小不小于nSpecLength
	** @Param nSpecLength: 指定的字节数
    ** @Param nTimeOut: 连接超时,单位: 毫秒
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @Ret : socket连接返回SOCKET_ERROR时返回false
	*/
	static bool SendToSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode);

	/*
	** 从指定的Socket连接接收指定字节数,
	** 如果从Socket中读取的字节数无法达到指标,则阻塞挂起,
	** 直到接收到指定的字节数后, 函数返回.
	** @Param s: 指定的Socket连接
	** @Param pBuffer: 接收缓冲区, 大小不小于nSpecLength
	** @Param nSpecLength: 指定的字节数
    ** @Param nTimeOut: 连接超时,单位: 毫秒
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @Ret : socket连接返回SOCKET_ERROR时返回false
	*/
    static bool RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode);

	/*
	** 获取指定socket的IP信息
	** @Param s: 指定的socket句柄
	** @Param addr: SOCKADDR_IN结构体
	** @Param nErrCode: 由WSAGetLastError()返回的错误码
	** @Ret : 操作成功返回true,失败返回false
	*/
	static bool GetAddressBySocket(SOCKET s, SOCKADDR_IN & addr, int& nErrCode);

};


