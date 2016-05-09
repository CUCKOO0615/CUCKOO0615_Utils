#include "StdAfx.h"
#include "TinyXmlEx.h"



TinyXmlEx::ErrEnums TinyXmlEx::SetText(TiXmlNode* parTiXmlNode, const char* szText)
{
    if (NULL == parTiXmlNode || NULL == szText)
        return TINYXMLEX_PARAM_NULLOREMPTY;

    TiXmlNode* pFirstChild = parTiXmlNode->FirstChild();

    if (NULL == pFirstChild)
    {
        TiXmlText * pTextNode = new TiXmlText(szText);
        parTiXmlNode->LinkEndChild(pTextNode);
        return TINYXMLEX_OK;
    }

    if (TiXmlNode::TINYXML_TEXT == pFirstChild->Type())
    {
        pFirstChild->SetValue(szText);
        return TINYXMLEX_OK;
    }
    return TINYXMLEX_SETTEXT_FAILED;
}

TiXmlHandle TinyXmlEx::GetHandle(const TiXmlHandle& hRootHandle, const char* szXmlPath)
{
    int nStrLength = strlen(szXmlPath);
    if (0 == nStrLength)
        return hRootHandle.FirstChild("7F99E076-EB22-4BC3-BFC3-BB5B5130A91C_CUCKOO0615");  //·µ»Ø¿Õhandle

    int nPosBeg = 0;
    if (szXmlPath[0] == '/')
        ++nPosBeg;
    if (szXmlPath[nStrLength - 1] == '/')
        --nStrLength;
    int nPosEnd = nPosBeg;

    char szBuf[MAX_PATH] = { 0 };
    TiXmlHandle hRet = hRootHandle;
    while (nPosEnd != nStrLength + 1)
    {
        if ('/' == szXmlPath[nPosEnd] || '\0' == szXmlPath[nPosEnd])
        {
            memset(szBuf, 0x00, MAX_PATH);
            memcpy(szBuf, szXmlPath + nPosBeg, nPosEnd - nPosBeg);
            hRet = hRet.FirstChild(szBuf);

            ++nPosEnd;
            nPosBeg = nPosEnd;
            continue;
        }
        ++nPosEnd;
    }
    return hRet;
}

TinyXmlEx::ErrEnums TinyXmlEx::CreateNodes(TiXmlElement* pRootElem, const char* szXmlPath, bool bMergeExistPath)
{
    if (NULL == szXmlPath || 0 == strlen(szXmlPath) || NULL == pRootElem)
        return TINYXMLEX_PARAM_NULLOREMPTY;

    int nStrLength = strlen(szXmlPath);
    int nPosBeg = 0;
    if (szXmlPath[0] == '/')
        ++nPosBeg;
    if (szXmlPath[nStrLength - 1] == '/')
        --nStrLength;
    int nPosEnd = nPosBeg;
    char szBuf[MAX_PATH] = { 0 };
    TiXmlElement* pWalker = pRootElem;

    while (nPosEnd != nStrLength + 1)
    {
        if ('/' == szXmlPath[nPosEnd] || '\0' == szXmlPath[nPosEnd])
        {
            memset(szBuf, 0x00, MAX_PATH);
            memcpy(szBuf, szXmlPath + nPosBeg, nPosEnd - nPosBeg);

            if (bMergeExistPath)
            {
                TiXmlElement * pCheck = pWalker->FirstChildElement(szBuf);
                if (NULL != pCheck)
                {
                    pWalker = pCheck;
                    ++nPosEnd;
                    nPosBeg = nPosEnd;
                    continue;
                }
            }

            TiXmlElement* pNewElem = new TiXmlElement(szBuf);
            if (NULL == pNewElem)
                return TINYXMLEX_CREATENODE_FAILED;

            TiXmlNode* pRet = pWalker->LinkEndChild(pNewElem);
            if (NULL == pRet)
                return TINYXMLEX_LINKENDCHILD_FAILED;

            pWalker = pRet->ToElement();
            ++nPosEnd;
            nPosBeg = nPosEnd;
            continue;
        }
        ++nPosEnd;
    }
    return TINYXMLEX_OK;
}

const char* TinyXmlEx::GetText(TiXmlNode* pTiXmlNode, const char* szDefault /*= NULL*/)
{
    if (!pTiXmlNode)
        return szDefault;

    TiXmlElement* pTiXmlElem = pTiXmlNode->ToElement();
    if (!pTiXmlElem)
        return szDefault;

    const char* szTextValue = pTiXmlElem->GetText();
    return szTextValue ? szTextValue : szDefault;
}
