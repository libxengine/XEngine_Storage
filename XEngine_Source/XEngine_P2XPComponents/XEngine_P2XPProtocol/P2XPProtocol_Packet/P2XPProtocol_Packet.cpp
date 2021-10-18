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
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的协议头
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.四：pSt_P2XPPeer
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输出解析后的节点信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_Common(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode /* = 0 */, LPCTSTR lpszMsgBuffer /* = NULL */)
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
	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_Lan
函数功能：响应同步局域网地址列表
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的协议头
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
 参数.五：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_Lan(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pppSt_ListClients) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
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

	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_WLan
函数功能：响应同步局域网所有地址列表
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的协议头
 参数.二：pStl_ListClients
  In/Out：In
  类型：容器指针
  可空：N
  意思：客户端列表
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.四：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_WLan(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, list<XENGINE_P2XPPEER_PROTOCOL>* pStl_ListClients, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == pStl_ListClients) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		P2XPProtocol_IsErrorOccur = FALSE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	unordered_map<string, list<P2XPPROTOCOL_LANPACKET>> stl_MapClient;
	//首先处理公网下的局域网网段列表
	for (auto stl_ListIterator = pStl_ListClients->begin(); stl_ListIterator != pStl_ListClients->end(); stl_ListIterator++)
	{
		TCHAR tszClientAddr[128];
		XENGINE_LIBADDR st_LibAddr;
		P2XPPROTOCOL_LANPACKET st_LANPacket;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
		memset(&st_LANPacket, '\0', sizeof(P2XPPROTOCOL_LANPACKET));
		//分割
		BaseLib_OperatorIPAddr_IsIPV4Addr(stl_ListIterator->tszPrivateAddr, &st_LibAddr);
		_stprintf(tszClientAddr, _T("%d.%d.%d"), st_LibAddr.nIPAddr1, st_LibAddr.nIPAddr2, st_LibAddr.nIPAddr3);
		//赋值
		_tcscpy(st_LANPacket.tszUsername, stl_ListIterator->tszUserName);
		_tcscpy(st_LANPacket.tszClientAddr, stl_ListIterator->tszPrivateAddr);
		//判断是否存在
		unordered_map<string, list<P2XPPROTOCOL_LANPACKET>>::iterator stl_MapIterator = stl_MapClient.find(tszClientAddr);
		if (stl_MapIterator == stl_MapClient.end())
		{
			list<P2XPPROTOCOL_LANPACKET> stl_ListClient;

			stl_ListClient.push_back(st_LANPacket);
			stl_MapClient.insert(make_pair(tszClientAddr, stl_ListClient));
		}
		else
		{
			stl_MapIterator->second.push_back(st_LANPacket);
		}
	}
	//打包成JSON数据
	int nCount = 0;
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		Json::Value st_JsonList;
		for (auto stl_ListIterator = stl_MapIterator->second.begin(); stl_ListIterator != stl_MapIterator->second.end(); stl_ListIterator++)
		{
			Json::Value st_JsonObject;

			nCount++;
			st_JsonObject["ClientUser"] = stl_ListIterator->tszUsername;
			st_JsonObject["ClientAddr"] = stl_ListIterator->tszClientAddr;
			st_JsonList.append(st_JsonObject);
		}
		st_JsonArray["tszLANAddr"] = stl_MapIterator->first.c_str();
		st_JsonArray["tszLANList"] = st_JsonList;
	}
	st_JsonRoot["nCode"] = 0;
	st_JsonRoot["lpszMsgBuffer"] = "sucess";
	st_JsonRoot["ClientArray"] = st_JsonArray;
	st_JsonRoot["ClientCount"] = nCount;

	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
	}
	return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_User
函数功能：响应用户查询用户信息的请求协议封包函数
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的协议头
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
 参数.四：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_User(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL* pSt_PeerInfo, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_PeerInfo) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
        return FALSE;
    }
    Json::Value st_JsonRoot;
    Json::StreamWriterBuilder st_JsonBuilder;

    st_JsonRoot["Code"] = 0;
    st_JsonRoot["lpszMsgBuffer"] = "sucess";
    st_JsonRoot["dwConnectType"] = (Json::Value::UInt)pSt_PeerInfo->dwConnectType;
    st_JsonRoot["dwPeerType"] = pSt_PeerInfo->dwPeerType;
    st_JsonRoot["tszConnectAddr"] = pSt_PeerInfo->tszConnectAddr;
    st_JsonRoot["tszPrivateAddr"] = pSt_PeerInfo->tszPrivateAddr;
    st_JsonRoot["tszPublicAddr"] = pSt_PeerInfo->tszPublicAddr;
    st_JsonRoot["tszUserISP"] = pSt_PeerInfo->tszUserISP;
    st_JsonRoot["tszUserLocation"] = pSt_PeerInfo->tszUserLocation;
    st_JsonRoot["tszUserName"] = pSt_PeerInfo->tszUserName;

    st_JsonBuilder["emitUTF8"] = true;
	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_MsgLen = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
		memcpy(ptszMsgBuffer, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), *pInt_MsgLen);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), pSt_ProtocolHdr->unPacketSize);
	}
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_Packet_Connect
函数功能：请求连接打包函数
 参数.一：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要打包的协议头
 参数.二：pSt_IOProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入连接信息
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.四：pInt_MsgLen
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输入你的缓冲区大小,输出缓冲区真实大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_Packet::P2XPProtocol_Packet_Connect(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPIO_PROTOCOL* pSt_IOProtocol, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	P2XPProtocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		P2XPProtocol_IsErrorOccur = FALSE;
		P2XPProtocol_dwErrorCode = ERROR_XENGINE_P2XP_PROTOCOL_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;

	st_JsonRoot["tszSourceUser"] = pSt_IOProtocol->tszSourceUser;
	st_JsonRoot["tszDestUser"] = pSt_IOProtocol->tszDestUser;
	st_JsonRoot["tszConnectAddr"] = pSt_IOProtocol->tszConnectAddr;
	st_JsonRoot["nDestPort"] = pSt_IOProtocol->nDestPort;
	st_JsonRoot["bIsTcp"] = pSt_IOProtocol->bIsTcp;

	if (NULL == pSt_ProtocolHdr)
	{
		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
		memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), *pInt_MsgLen);
	}
	else
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();
		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), st_JsonRoot.toStyledString().c_str(), pSt_ProtocolHdr->unPacketSize);
	}
	return TRUE;
}