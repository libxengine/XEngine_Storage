#include "pch.h"
#include "P2XPProtocol_UserMsg/P2XPProtocol_UserMsg.h"
#include "P2XPProtocol_ServiceMsg/P2XPProtocol_ServiceMsg.h"
/********************************************************************
//	Created:	2013/1/18  12:37
//	File Name: 	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\NetEngine_P2XPProtocol.cpp
//	File Path:	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol
//	File Base:	NetEngine_P2XPProtocol
//	File Ext:	cpp
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	导出函数实现
//	History:
*********************************************************************/
BOOL P2XPProtocol_IsErrorOccur = FALSE;
DWORD P2XPProtocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CP2XPProtocol_ServiceMsg m_ServiceMsg;
CP2XPProtocol_UserMsg m_UserMsg;
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD P2XPProtocol_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return P2XPProtocol_dwErrorCode;
}
/************************************************************************/
/*                        服务协议导出                                  */
/************************************************************************/
extern "C" BOOL P2XPProtocol_ServiceMsg_ResponseJson(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * ptszMsgBuffer, int* pInt_Len, int nCode, LPCTSTR lpszCodeMsg)
{
	return m_ServiceMsg.P2XPProtocol_ServiceMsg_ResponseJson(pSt_ProtocolHdr, ptszMsgBuffer, pInt_Len, nCode, lpszCodeMsg);
}
extern "C" BOOL P2XPProtocol_ServiceMsg_ResponseLan(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * **pppSt_ListClients, int nListCount, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_ServiceMsg.P2XPProtocol_ServiceMsg_ResponseLan(pSt_ProtocolHdr, pppSt_ListClients, nListCount, ptszMsgBuffer, pInt_Len);
}
extern "C" BOOL P2XPProtocol_ServiceMsg_ResponseQueryUser(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * pSt_PeerInfo, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_ServiceMsg.P2XPProtocol_ServiceMsg_ResponseQueryUser(pSt_ProtocolHdr, pSt_PeerInfo, ptszMsgBuffer, pInt_Len);
}
/************************************************************************/
/*                        用户协议导出                                  */
/************************************************************************/
extern "C" BOOL P2XPProtocol_UserMsg_RequestLogin(XENGINE_P2XPPEER_PROTOCOL * pSt_ClientInfo, XENGINE_PROTOCOL_USERAUTH * pSt_UserLogin, TCHAR * ptszMsgBuffer, int* pInt_Len, WORD wPacketSerial)
{
	return m_UserMsg.P2XPProtocol_UserMsg_RequestLogin(pSt_ClientInfo, pSt_UserLogin, ptszMsgBuffer, pInt_Len, wPacketSerial);
}
extern "C" BOOL P2XPProtocol_UserMsg_RequestConnect(XENGINE_P2XPIO_PROTOCOL * pSt_ClientIONet, TCHAR * ptszMsgBuffer, int* pInt_Len, WORD wPacketSerial)
{
	return m_UserMsg.P2XPProtocol_UserMsg_RequestConnect(pSt_ClientIONet, ptszMsgBuffer, pInt_Len, wPacketSerial);
}
extern "C" BOOL P2XPProtocol_UserMsg_RequestLan(TCHAR * ptszMsgBuffer, int* pInt_Len, BOOL bWlan, WORD wPacketSerial)
{
	return m_UserMsg.P2XPProtocol_UserMsg_RequestLan(ptszMsgBuffer, pInt_Len, bWlan, wPacketSerial);
}
extern "C" BOOL P2XPProtocol_UserMsg_RequestQueryUser(XENGINE_P2XPPEER_PROTOCOL * pSt_Client, TCHAR * ptszMsgBuffer, int* pInt_Len, WORD wPacketSerial)
{
	return m_UserMsg.P2XPProtocol_UserMsg_RequestQueryUser(pSt_Client, ptszMsgBuffer, pInt_Len, wPacketSerial);
}
extern "C" BOOL P2XPProtocol_UserMsg_RequestHeartBeat(CHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_UserMsg.P2XPProtocol_UserMsg_RequestHeartBeat(ptszMsgBuffer, pInt_Len);
}
