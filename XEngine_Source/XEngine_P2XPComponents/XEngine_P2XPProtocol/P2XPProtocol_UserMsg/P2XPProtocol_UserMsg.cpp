#include "pch.h"
#include "P2XPProtocol_UserMsg.h"
/********************************************************************
//	Created:	2013/1/15  18:06
//	File Name: 	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_UserMsg\P2XPProtocol_UserMsg.cpp
//	File Path:	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_UserMsg
//	File Base:	P2XPProtocol_UserMsg
//	File Ext:	cpp
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	P2XP协议管理器，用户消息实现
//	History:
*********************************************************************/
CP2XPProtocol_UserMsg::CP2XPProtocol_UserMsg()
{

}
CP2XPProtocol_UserMsg::~CP2XPProtocol_UserMsg()
{

}
//////////////////////////////////////////////////////////////////////////
//                         公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：P2XPProtocol_UserMsg_RequestLogin
函数功能：构造一个请求登录数据包
 参数.一：pSt_ClientInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入你构造的客户端信息
 参数.二：pSt_UserLogin
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入你构造的客户端用户信息
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出组好包的数据
 参数.四：pInt_Len
  In/Out：In
  类型：整数型指针
  可空：N
  意思：导出组好包的数据长度
 参数.五：wPacketSerial
  In/Out：In
  类型：无符号短整数型
  可空：Y
  意思：设置这个包的序列,可以不设置,服务器返回的包序列也是这个
返回值
  类型：逻辑型
  意思：是否成功构造
备注：
*********************************************************************/
BOOL CP2XPProtocol_UserMsg::P2XPProtocol_UserMsg_RequestLogin(XENGINE_P2XPPEER_PROTOCOL* pSt_ClientInfo, XENGINE_PROTOCOL_USERAUTH* pSt_UserLogin, TCHAR* ptszMsgBuffer, int* pInt_Len, WORD wPacketSerial)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_ClientInfo) || (NULL == pSt_UserLogin) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_USERMSG_REQUESTLOGIN_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr,'\0',sizeof(XENGINE_PROTOCOLHDR));

    if (0 != wPacketSerial)
    {
        st_ProtocolHdr.wPacketSerial = wPacketSerial;
    }
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
    st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH) + sizeof(XENGINE_P2XPPEER_PROTOCOL);
    st_ProtocolHdr.byIsReply = TRUE;

    *pInt_Len = st_ProtocolHdr.unPacketSize + sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_UserLogin, sizeof(XENGINE_PROTOCOL_USERAUTH));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_USERAUTH), pSt_ClientInfo,sizeof(XENGINE_P2XPPEER_PROTOCOL));
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_UserMsg_RequestConnect
函数功能：请求连接到一个网络服务
 参数.一：pSt_ClientIONet
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入你构造的P2XP网络IO结构
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出组好包的数据
 参数.三：pInt_Len
  In/Out：In
  类型：整数型指针
  可空：N
  意思：导出组好包的数据长度
 参数.四：wPacketSerial
  In/Out：In
  类型：无符号短整数型
  可空：Y
  意思：设置这个包的序列,可以不设置,服务器返回的包序列也是这个
返回值
  类型：逻辑型
  意思：是否成功构造
备注：XENGINE_P2XPIO_PROTOCOL所有值必须填写,否则无法使用
*********************************************************************/
BOOL CP2XPProtocol_UserMsg::P2XPProtocol_UserMsg_RequestConnect(XENGINE_P2XPIO_PROTOCOL *pSt_ClientIONet, TCHAR *ptszMsgBuffer, int *pInt_Len, WORD wPacketSerial)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == pSt_ClientIONet) || (NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_USERMSG_REQUESTIONET_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    if (0 != wPacketSerial)
    {
        st_ProtocolHdr.wPacketSerial = wPacketSerial;
    }
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT;
    st_ProtocolHdr.unPacketSize = sizeof(XENGINE_P2XPIO_PROTOCOL);
    st_ProtocolHdr.byIsReply = TRUE;

    *pInt_Len = st_ProtocolHdr.unPacketSize + sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ClientIONet, sizeof(XENGINE_P2XPIO_PROTOCOL));
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_UserMsg_RequestLan
函数功能：请求同步局域网列表封装函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的数据包
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型
  可空：N
  意思：导出数据包长度
 参数.二：bWlan
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：请求公网下地址列表还是局域网下地址列表,默认局域网
 参数.二：wPacketSerial
  In/Out：In
  类型：无符号短整数型
  可空：Y
  意思：输入这个封包的序列号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_UserMsg::P2XPProtocol_UserMsg_RequestLan(TCHAR *ptszMsgBuffer, int *pInt_Len, BOOL bWlan,WORD wPacketSerial /* = 0 */)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_USERMSG_REQUESTLIST_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    if (0 != wPacketSerial)
    {
        st_ProtocolHdr.wPacketSerial = wPacketSerial;
    }

    if (bWlan)
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQWLANLIST;
    }
    else
    {
        st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQLANLIST;
    }
    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_P2XP;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
    st_ProtocolHdr.unPacketSize = 0;
    st_ProtocolHdr.byIsReply = TRUE;

    *pInt_Len = sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    return TRUE;
}
/********************************************************************
函数名称：P2XPProtocol_UserMsg_RequestHeartBeat
函数功能：封装一个心跳请求的包
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：导出封装好的缓冲区
 参数.二：pInt_Len
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：导出缓冲区长度
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CP2XPProtocol_UserMsg::P2XPProtocol_UserMsg_RequestHeartBeat(TCHAR *ptszMsgBuffer, int *pInt_Len)
{
    P2XPProtocol_IsErrorOccur = FALSE;

    if ((NULL == ptszMsgBuffer) || (NULL == pInt_Len))
    {
        P2XPProtocol_IsErrorOccur = FALSE;
        P2XPProtocol_dwErrorCode = ERROR_NETENGINE_P2XP_PROTOCOL_USERMSG_REQUESTHB_PARAMENT;
        return FALSE;
    }
    XENGINE_PROTOCOLHDR st_ProtocolHdr;
    memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

    st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
    st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT;
    st_ProtocolHdr.byVersion = 1;
    st_ProtocolHdr.unPacketSize = 0;
    st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

    *pInt_Len = sizeof(XENGINE_PROTOCOLHDR);
    memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                         保护函数
//////////////////////////////////////////////////////////////////////////
