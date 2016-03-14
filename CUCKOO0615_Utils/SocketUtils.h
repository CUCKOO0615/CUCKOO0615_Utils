
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
#define ERRMSGTABLE_LENGTH 41
#endif

typedef struct
{
	long lRetVal;
	const char* szErrMsg;
}ERRMSG_TABLE;
static const ERRMSG_TABLE ErrMsgTable[] =
{
	//Init
	{ WSASYSNOTREADY, "WSASYSNOTREADY" },
	{ WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED" },
	{ WSAEINPROGRESS, "WSAEINPROGRESS" },
	{ WSAEPROCLIM, "WSAEPROCLIM" },
	{ WSAEFAULT, "WSAEFAULT" },

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
public:
	SocketUtils(void);
	~SocketUtils(void);

private:
	static bool m_bIsInited;
	static char m_szInitFailedErrMsg[ERRMSG_LENGTH];

	char m_szErrMsg[ERRMSG_LENGTH];
	void SetErrMsg(const char* szErrMsg)   { strcpy(m_szErrMsg, szErrMsg); }
	void SetErrMsgEx(const char* szErrMsg) { strcat(m_szErrMsg, szErrMsg); }

	static const char* QueryErrMsg(size_t nErrCode);

public:

	/*
	** 初始化WinSocket, 初始化WSA资源
	** @Ret : 操作成功返回true,失败返回false,由GetInitErrMsg()获取相关错误信息
	*/
	static bool InitSocketUtils();

	/*
	** 释放WSA资源等
	** 调用后如需继续使用socket,必须重新调用InitSocketUtils()进行初始化
	** @Ret : 操作成功返回true,失败返回false,由GetInitErrMsg()获取相关错误信息
	*/
	static bool ReleaseSocketUtils();

	/*
	**获取初始化过程中的错误信息
	** @Ret : 包含错误信息的字符串
	*/
	static const char* GetInitErrMsg() { return m_szInitFailedErrMsg; }

	/*
	** 创建一个TCP客户端连接
	** @param szServerAddr： 服务器IP地址
	** @param usPort： 服务器端口号
	** @return: 连接成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	SOCKET CreateClientSocket_TCP(const char* szServerAddr = "127.0.0.1", u_short usPort = 10086);

	/*
	** 创建一个TCP服务端连接
	** @param usPort： 要绑定的端口
	** @return： 建立成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	SOCKET CreateServerSocket_TCP(u_short usPort = 10086);

	/*
	** 在指定的端口号范围内尝试建立一个TCP服务端连接
	** @Param usPortMin: 最小端口号
	** @Param usPortMax: 最大端口号
	** @Param usPort: 操作成功时返回当前正在监听的端口号
	** @Ret: 建立成功，返回有效的SOCKET，否则，返回INVALID_SOCKET
	*/
	SOCKET CreateServerSocket_TCP_Ex(u_short usPortMin, u_short usPortMax, u_short & usPort);

	bool ListenAt(SOCKET skSrvSock, int nBackLog);

	/*
	** 向指定的Socket连接发送指定的字节数
	** @Param s: 指定的Socket连接
	** @Param pBuffer: 发送缓冲区,大小不小于nSpecLength
	** @Param nSpecLength: 指定的字节数
	** @Ret : socket连接返回SOCKET_ERROR时返回false
	*/
	bool SendToSocket(SOCKET s, char* pBuffer, int nSpecLength);

	/*
	** 从指定的Socket连接接收指定字节数,
	** 如果从Socket中读取的字节数无法达到指标,则阻塞挂起,
	** 直到接收到指定的字节数后, 函数返回.
	** @Param s: 指定的Socket连接
	** @Param pBuffer: 接收缓冲区, 大小不小于nSpecLength
	** @Param nSpecLength: 指定的字节数
	** @Ret : socket连接返回SOCKET_ERROR时返回false
	*/
	bool RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength);


	//获取错误信息
	const char* GetErrMsg() { return m_szErrMsg; }
};


