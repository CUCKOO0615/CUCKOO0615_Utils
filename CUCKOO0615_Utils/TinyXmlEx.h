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
        //操作成功
        TINYXMLEX_OK,
        //设置文本节点失败
        TINYXMLEX_SETTEXT_FAILED,
        //参数为空
        TINYXMLEX_PARAM_NULLOREMPTY,
        //追加节点失败
        TINYXMLEX_LINKENDCHILD_FAILED,
        //创建节点失败
        TINYXMLEX_CREATENODE_FAILED
    };

    /* 
    ** 设置TixmlNode节点的文本值
    ** @param pTiXmlNode: 要设置文本的Xml节点
    ** @param szText: 节点文本
    ** @return: 操作成功返回 TINYXMLEX_OK, 操作失败返回相应的错误枚举值
    ** - 返回TINYXMLEX_SETTEXT_FAILED的原因是有子节点存在,但子节点类型不是TINYXML_TEXT
    ** - 因此不能向该节点设置Text文本
    */
    ErrEnums SetText(TiXmlNode* pTiXmlNode, const char* szText);


    const char* GetText(TiXmlNode* pTiXmlNode, const char* szDefault = NULL);

    /* 
    ** 根据指定的搜索路径返回末端节点的句柄
    ** @param hRootHandle: 搜索起始节点的句柄
    ** @param szXmlPath: 搜索路径, 搜索路径各节点间以'/'分隔, 例: "XXXX/XXX/XXX/XXX/XXX"
    ** @return: 操作成功返回末端节点的句柄, 操作失败返回无效的句柄(To函数转换后为空指针) 
    */
    TiXmlHandle GetHandle(const TiXmlHandle& hRootHandle, const char* szXmlPath);

    /* 
    ** 创建指定的节点路径
    ** @param pRootElem: 路径的父节点
    ** @param szXmlPath: XML节点路径,各节点间以'/'分隔, 例: "XXXX/XXX/XXX/XXX/XXX"
    ** @param bMergeExistPath: 默认false,如果路径中的某节点已存在,则创建它的兄弟节点;
    ** @return: 操作成功返回 TINYXMLEX_OK, 操作失败返回相应的错误枚举值
    */
    ErrEnums CreateNodes(TiXmlElement* pRootElem, const char* szXmlPath, bool bMergeExistPath = false);


};

