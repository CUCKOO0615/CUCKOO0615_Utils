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

#include <string.h>
#include "./TinyXml/tinyxml.h"

namespace TinyXmlEx
{
    enum ErrEnums
    {
        //�����ɹ�
        TINYXMLEX_OK,
        //�����ı��ڵ�ʧ��
        TINYXMLEX_SETTEXT_FAILED,
        //����Ϊ��
        TINYXMLEX_PARAM_NULLOREMPTY,
        //׷�ӽڵ�ʧ��
        TINYXMLEX_LINKENDCHILD_FAILED,
        //�����ڵ�ʧ��
        TINYXMLEX_CREATENODE_FAILED
    };

    /* 
    ** ����TixmlNode�ڵ���ı�ֵ
    ** @param pTiXmlNode: Ҫ�����ı���Xml�ڵ�
    ** @param szText: �ڵ��ı�
    ** @return: �����ɹ����� TINYXMLEX_OK, ����ʧ�ܷ�����Ӧ�Ĵ���ö��ֵ
    ** - ����TINYXMLEX_SETTEXT_FAILED��ԭ�������ӽڵ����,���ӽڵ����Ͳ���TINYXML_TEXT
    ** - ��˲�����ýڵ�����Text�ı�
    */
    ErrEnums SetText(TiXmlNode* pTiXmlNode, const char* szText);


    const char* GetText(TiXmlNode* pTiXmlNode, const char* szDefault = NULL);

    /* 
    ** ����ָ��������·������ĩ�˽ڵ�ľ��
    ** @param hRootHandle: ������ʼ�ڵ�ľ��
    ** @param szXmlPath: ����·��, ����·�����ڵ����'/'�ָ�, ��: "XXXX/XXX/XXX/XXX/XXX"
    ** @return: �����ɹ�����ĩ�˽ڵ�ľ��, ����ʧ�ܷ�����Ч�ľ��(To����ת����Ϊ��ָ��) 
    */
    TiXmlHandle GetHandle(const TiXmlHandle& hRootHandle, const char* szXmlPath);

    /* 
    ** ����ָ���Ľڵ�·��
    ** @param pRootElem: ·���ĸ��ڵ�
    ** @param szXmlPath: XML�ڵ�·��,���ڵ����'/'�ָ�, ��: "XXXX/XXX/XXX/XXX/XXX"
    ** @param bMergeExistPath: Ĭ��false,���·���е�ĳ�ڵ��Ѵ���,�򴴽������ֵܽڵ�;
    ** @return: �����ɹ����� TINYXMLEX_OK, ����ʧ�ܷ�����Ӧ�Ĵ���ö��ֵ
    */
    ErrEnums CreateNodes(TiXmlElement* pRootElem, const char* szXmlPath, bool bMergeExistPath = false);


};

