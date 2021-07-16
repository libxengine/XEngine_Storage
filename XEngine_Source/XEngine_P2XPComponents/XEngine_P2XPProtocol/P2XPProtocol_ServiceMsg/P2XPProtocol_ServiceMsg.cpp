#include "pch.h"
#include "./P2XPProtocol_ServiceMsg.h"
/********************************************************************
//	Created:	2018/11/7   10:55
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_ServiceMsg\P2XPProtocol_ServiceMsg.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_ServiceMsg
//	File Base:	P2XPProtocol_ServiceMsg
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	服务器消息协议
//	History:
*********************************************************************/
CP2XPProtocol_ServiceMsg::CP2XPProtocol_ServiceMsg()
{

}
CP2XPProtocol_ServiceMsg::~CP2XPProtocol_ServiceMsg()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPProtocol_ServiceMsg_ResponseJson
函数功能：处理一个相应包,用于告诉请求端请求的内容成功还是失败!
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要处理的协议头
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出组号包的数据
 参数.三：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出组号包的数据大小
 参数.四：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入要返回的状态码，默认为0 成功
 参数.五：lpszCodeMsg
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入状态码意思，可以为NULL
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_ServiceMsg::P2XPProtocol_ServiceMsg_ResponseJson(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, TCHAR *ptszMsgBuffer, int *pInt_Len, int nCode, LPCTSTR lpszCodeMsg)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_ProtocolHdr) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_SVRMSG_RESPONSEJSON_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["Code"] = nCode;
    if (NULL != lpszCodeMsg)
    {
        st_JsonRoot["CodeMsg"] = lpszCodeMsg;
    }

    pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
    *pInt_Len = pSt_ProtocolHdr->unPacketSize + sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_ServiceMsg_ResponseLan
函数功能：响应同步局域网地址列表
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：请求的协议头
 参数.二：pppSt_ListClients
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入局域网地址信息列表
 参数.三：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：局域网地址列表个数
 参数.四：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.五：pInt_Len
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_ServiceMsg::P2XPProtocol_ServiceMsg_ResponseLan(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR *ptszMsgBuffer, int *pInt_Len)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_ProtocolHdr) || (NULL == pppSt_ListClients) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_SVRMSG_RESPONSELAN_PARAMENT;
        return FALSE;
    }

    Json::Value st_JsonRoot;
    Json::Value st_JsonArray;
    for (int i = 0; i < nListCount; i++)
    {
        Json::Value st_JsonObject;
        st_JsonObject["ClientUser"] = (*pppSt_ListClients)[i]->tszUserName;
        st_JsonObject["ClientAddr"] = (*pppSt_ListClients)[i]->tszPrivateAddr;
        st_JsonArray.append(st_JsonObject);
    }
    st_JsonRoot["Code"] = 0;
    st_JsonRoot["ClientArray"] = st_JsonArray;
    st_JsonRoot["ClientCount"] = nListCount;
    pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();

    int nLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
    if (nLen > *pInt_Len)
    {
        *pInt_Len = nLen;
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_SVRMSG_RESPONSELAN_LEN;
        return FALSE;
    }
    *pInt_Len = nLen;
    memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_ServiceMsg_ResponseQueryUser
函数功能：响应用户查询用户信息的请求协议封包函数
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：请求的协议头
 参数.二：pSt_PeerInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入获取到的用户信息
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.四：pInt_Len
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_ServiceMsg::P2XPProtocol_ServiceMsg_ResponseQueryUser(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL *pSt_PeerInfo, TCHAR *ptszMsgBuffer, int *pInt_Len)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_ProtocolHdr) || (NULL == pSt_PeerInfo) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_SVRMSG_REPQUERYUSER_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["Code"] = 0;
    st_JsonRoot["dwConnectType"] = (Json::Value::UInt)pSt_PeerInfo->dwConnectType;
    st_JsonRoot["dwPeerType"] = pSt_PeerInfo->dwPeerType;
    st_JsonRoot["tszConnectAddr"] = pSt_PeerInfo->tszConnectAddr;
    st_JsonRoot["tszPrivateAddr"] = pSt_PeerInfo->tszPrivateAddr;
    st_JsonRoot["tszPublicAddr"] = pSt_PeerInfo->tszPublicAddr;
    st_JsonRoot["tszUserArea"] = pSt_PeerInfo->tszUserArea;
    st_JsonRoot["tszUserLocation"] = pSt_PeerInfo->tszUserLocation;
    st_JsonRoot["tszUserName"] = pSt_PeerInfo->tszUserName;

    pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();

    *pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
    memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                         保护函数
//////////////////////////////////////////////////////////////////////////
