#pragma once
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/09/22
//��˵������SOCKET��װ
//*************************************************
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

enum SingleSocketType
{
    SS_NotSet, //δ����
    SS_Server, //������ģʽ
    SS_Client  //�ͻ���ģʽ
};

class SingleSocket
{
public:
    /*
    ** ��ʼ��
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false, ��GetErrMsg()��ȡ������Ϣ
    */
    bool Init();

    // ����
    void Reset();
    
    /*
    ** ����Socket
    ** @Param emType: �μ� SingleSocketType
    ** @Param szIpAddr: ������IP��ַ, ʹ��SS_Serverģʽʱ�ò�����Ч
    ** @Param usPort: SS_Serverģʽʱ�ò���ָ�������˿�, SS_Clientģʽʱ�ò���ָ���������˿�
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false
    */
    bool CreateSocket(SingleSocketType emType, const char* szIpAddr, u_short usPort);
    
    /*
    ** ��������
    ** @Param nBackLog: �ȴ�����
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false
    */
    bool StartListening(int nBackLog = 5);
        
    /*
    ** ���ó�ʱ
    ** @Param nMilliseconds: ��ʱʱ��,��λ:����. Ĭ��ֵINFINIE, ������ģʽ
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false
    */
    bool SetSocketTimeOut(int nMilliseconds = INFINITE);
    
    /*
    ** �������ݵ�������, δ�ﵽָ������ʱ�����ȴ�, SS_Clientģʽʱ����
    ** @Param pBuffer: ������ָ�� 
    ** @Param nLength: �������ݵĳ���
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false
    */
    bool SendToServer(char* pBuffer, int nLength);
    
    /*
    ** �ӷ�������������, δ�ﵽָ������ʱ�����ȴ�, SS_Clientģʽʱ���� 
    ** @Param pBuffer: ������ָ��
    ** @Param nLength: �������ݵĳ���
    ** @Ret: �����ɹ�����true, ʧ�ܷ���false
    */
    bool RecvFromServer(char* pBuffer, int nLength);

    /*
    ** ��ȡ��������IP��Ϣ, SS_Clientģʽʱ���� 
    ** @Param addr: SOCKADDR_IN�ṹ��
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false
    */
    bool GetAddressBySocket(SOCKADDR_IN & addr);

    //��ȡ������Ϣ
    const char* GetErrMsg() { return m_szErrMsg; }

    // STATIC FUNC ///////////////////////////////////////////////////////////
    /*
    ** ��ָ����Socket���ӷ���ָ�����ֽ���
    ** @Param s: ָ����Socket����
    ** @Param pBuffer: ���ͻ�����,��С��С��nSpecLength
    ** @Param nSpecLength: ָ�����ֽ���
    ** @Param nTimeOut: ���ӳ�ʱ,��λ: ����
    ** @Param nErrCode: ��WSAGetLastError()���صĴ�����
    ** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
    */
    static bool SendToSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode);

    /*
    ** ��ָ����Socket���ӽ���ָ���ֽ���,
    ** �����Socket�ж�ȡ���ֽ����޷��ﵽָ��,����������,
    ** ֱ�����յ�ָ�����ֽ�����, ��������.
    ** @Param s: ָ����Socket����
    ** @Param pBuffer: ���ջ�����, ��С��С��nSpecLength
    ** @Param nSpecLength: ָ�����ֽ���
    ** @Param nTimeOut: ���ӳ�ʱ,��λ: ����
    ** @Param nErrCode: ��WSAGetLastError()���صĴ�����
    ** @Ret : socket���ӷ���SOCKET_ERRORʱ����false
    */
    static bool RecvFromSocket(SOCKET s, char* pBuffer, int nSpecLength, int nTimeOut, int& nErrCode);

    /*
    ** ��ȡָ��socket��IP��Ϣ
    ** @Param s: ָ����socket���
    ** @Param addr: SOCKADDR_IN�ṹ��
    ** @Param nErrCode: ��WSAGetLastError()���صĴ�����
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false
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

