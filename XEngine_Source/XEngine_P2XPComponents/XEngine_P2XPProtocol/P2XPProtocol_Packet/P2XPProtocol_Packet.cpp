#include "pch.h"
#include "P2XPProtocol_Packet.h"
/********************************************************************
//    Created:     2021/07/19  10:23:14
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Packet\P2XPProtocol_Packet.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Packet
//    File Base:   P2XPProtocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     打包类
//    History:
*********************************************************************/
CP2XPProtocol_Packet::CP2XPProtocol_Packet()
{

}
CP2XPProtocol_Packet::~CP2XPProtocol_Packet()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPProtocol_Parse_Login
函数功能：登录解析函数
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：pSt_P2XPPeer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输出解析后的节点信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_Common(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode /* = 0 */, LPCTSTR lpszMsgBuffer /* = NULL */)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		P2XPProtocol_IsErrorOccur = TRUE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["nCode"] = 0;
	if (NULL != lpszMsgBuffer)
	{
		st_JsonRoot["lpszMsgBuffer"] = lpszMsgBuffer;
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_Lan
函数功能：响应同步局域网地址列表
 参数.一：pppSt_ListClients
  In/Out：In
  类型：三级指针
  可空：N
  意思：输入局域网地址信息列表
 参数.二：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：局域网地址列表个数
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
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_Lan(XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR* ptszMsgBuffer, int* pInt_Len)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pppSt_ListClients) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
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
    st_JsonRoot["nCode"] = 0;
    st_JsonRoot["lpszMsgBuffer"] = "sucess";
    st_JsonRoot["ClientArray"] = st_JsonArray;
    st_JsonRoot["ClientCount"] = nListCount;

    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_User
函数功能：响应用户查询用户信息的请求协议封包函数
 参数.一：pSt_PeerInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入获取到的用户信息
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.三：pInt_Len
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_User(XENGINE_P2XPPEER_PROTOCOL* pSt_PeerInfo, TCHAR* ptszMsgBuffer, int* pInt_Len)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_PeerInfo) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;

    st_JsonRoot["Code"] = 0;
    st_JsonRoot["lpszMsgBuffer"] = "sucess";
    st_JsonRoot["dwConnectType"] = (Json::Value::UInt)pSt_PeerInfo->dwConnectType;
    st_JsonRoot["dwPeerType"] = pSt_PeerInfo->dwPeerType;
    st_JsonRoot["tszConnectAddr"] = pSt_PeerInfo->tszConnectAddr;
    st_JsonRoot["tszPrivateAddr"] = pSt_PeerInfo->tszPrivateAddr;
    st_JsonRoot["tszPublicAddr"] = pSt_PeerInfo->tszPublicAddr;
    st_JsonRoot["tszUserArea"] = pSt_PeerInfo->tszUserArea;
    st_JsonRoot["tszUserLocation"] = pSt_PeerInfo->tszUserLocation;
    st_JsonRoot["tszUserName"] = pSt_PeerInfo->tszUserName;

    *pInt_Len = st_JsonRoot.toStyledString().length();
    memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
    return TRUE;
}