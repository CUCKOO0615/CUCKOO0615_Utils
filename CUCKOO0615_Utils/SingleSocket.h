#pragma once
//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/09/22
//◇说明：单SOCKET封装
//*************************************************
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

enum SingleSocketType
{
    SS_NotSet, //未设置
    SS_Server, //服务器模式
    SS_Client  //客户端模式
};

class SingleSocket
{
public:
    /*
    ** 初始化
    ** @Ret: 操作成功返回true, 失败返回false, 由GetErrMsg()获取错误信息
    */
    bool Init();

    // 重置
    void Reset();
    
    /*
    ** 创建Socket
    ** @Param emType: 参见 SingleSocketType
    ** @Param szIpAddr: 服务器IP地址, 使用SS_Server模式时该参数无效
    ** @Param usPort: SS_Server模式时该参数指定监听端口, SS_Client模式时该参数指定服务器端口
    ** @Ret: 操作成功返回true, 失败返回false
    */
    bool CreateSocket(SingleSocketType emType, const char* szIpAddr, u_short usPort);
    
    /*
    ** 启动监听
    ** @Param nBackLog: 等待队列
    ** @Ret: 操作成功返回true, 失败返回false
    */
    bool StartListening(int nBackLog = 5);
        
    /*
    ** 设置超时
    ** @Param nMilliseconds: 超时时间,单位:毫秒. 默认值INFINIE, 即阻塞模式
    ** @Ret: 操作成功返回true, 失败返回false
    */
    bool SetSocketTimeOut(int nMilliseconds = INFINITE);
    
    /*
    ** 发送数据到服务器, 未达到指定长度时阻塞等待, SS_Client模式时可用
    ** @Param pBuffer: 缓冲区指针 
    ** @Param nLength: 请求数据的长度
    ** @Ret: 操作成功返回true, 失败返回false
    */
    bool SendToServer(char* pBuffer, int nLength);
    
    /*
    ** 从服务器接收数据, 未达到指定长度时阻塞等待, SS_Client模式时可用 
    ** @Param pBuffer: 缓冲区指针
    ** @Param nLength: 请求数据的长度
    ** @Ret: 操作成功返回true, 失败返回false
    */
    bool RecvFromServer(char* pBuffer, int nLength);

    /*
    ** 获取服务器端IP信息, SS_Client模式时可用 
    ** @Param addr: SOCKADDR_IN结构体
    ** @Ret : 操作成功返回true,失败返回false
    */
    bool GetAddressBySocket(SOCKADDR_IN & addr);

    //获取错误信息
    const char* GetErrMsg() { return m_szErrMsg; }

    // STATIC FUNC ///////////////////////////////////////////////////////////
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
        
    //////////////////////////////////////////////////////////////////////////

public:
    SingleSocket();
    ~SingleSocket();

private:
    bool m_bIsInited;
    SOCKET m_sSingleSocket;
    SingleSocketType m_emSocketType;

    char m_szErrMsg[ERRMSG_LENGTH];
    void SetErrMsg(const char* szErrMsg) { ::strcpy(m_szErrMsg, szErrMsg); };
    void FmtErrMsg(const char* szFormat, ...);

    const char* QueryErrMsg(size_t nErrCode);
};

