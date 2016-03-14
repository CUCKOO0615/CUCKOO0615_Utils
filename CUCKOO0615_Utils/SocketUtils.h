
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��14��
//��˵����Socket����
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
	** ��ʼ��WinSocket, ��ʼ��WSA��Դ
	** @Ret : �����ɹ�����true,ʧ�ܷ���false,��GetInitErrMsg()��ȡ��ش�����Ϣ
	*/
	static bool InitSocketUtils();

	/*
	** �ͷ�WSA��Դ��
	** ���ú��������ʹ��socket,�������µ���InitSocketUtils()���г�ʼ��
	** @Ret : �����ɹ�����true,ʧ�ܷ���false,��GetInitErrMsg()��ȡ��ش�����Ϣ
	*/
	static bool ReleaseSocketUtils();

	/*
	**��ȡ��ʼ�������еĴ�����Ϣ
	** @Ret : ����������Ϣ���ַ���
	*/
	static const char* GetInitErrMsg() { return m_szInitFailedErrMsg; }

	/*
	** ����һ��TCP�ͻ�������
	** @param szServerAddr�� ������IP��ַ
	** @param usPort�� �������˿ں�
	** @return: ���ӳɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	SOCKET CreateClientSocket_TCP(const char* szServerAddr = "127.0.0.1", u_short usPort = 10086);

	/*
	** ����һ��TCP���������
	** @param usPort�� Ҫ�󶨵Ķ˿�
	** @return�� �����ɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	SOCKET CreateServerSocket_TCP(u_short usPort = 10086);

	/*
	** ��ָ���Ķ˿ںŷ�Χ�ڳ��Խ���һ��TCP���������
	** @Param usPortMin: ��С�˿ں�
	** @Param usPortMax: ���˿ں�
	** @Param usPort: �����ɹ�ʱ���ص�ǰ���ڼ����Ķ˿ں�
	** @Ret: �����ɹ���������Ч��SOCKET�����򣬷���INVALID_SOCKET
	*/
	SOCKET CreateServerSocket_TCP_Ex(u_short usPortMin, u_short usPortMax, u_short & usPort);

	bool ListenAt(SOCKET skSrvSock, int nBackLog);

	/*
	** ��ָ����Socket���ӷ���ָ�����ֽ���
	** @Param s: ָ����Socket����
	** @Param pBuffer: ���ͻ�����,��С��С��nSpecLength
	** @Param nSpecLength: ָ�����ֽ���
	** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
	*/
	bool SendToSocket(SOCKET s, char* pBuffer, int nSpecLength);

	/*
	** ��ָ����Socket���ӽ���ָ���ֽ���,
	** �����Socket�ж�ȡ���ֽ����޷��ﵽָ��,����������,
	** ֱ�����յ�ָ�����ֽ�����, ��������.
	** @Param s: ָ����Socket����
	** @Param pBuffer: ���ջ�����, ��С��С��nSpecLength
	** @Param nSpecLength: ָ�����ֽ���
	** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
	*/
	bool RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength);


	//��ȡ������Ϣ
	const char* GetErrMsg() { return m_szErrMsg; }
};


