#include "pch.h"
#include "P2XPProtocol_Parse/P2XPProtocol_Parse.h"
#include "P2XPProtocol_Packet/P2XPProtocol_Packet.h"
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
CP2XPProtocol_Parse m_P2XPParse;
CP2XPProtocol_Packet m_P2XPPacket;
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
/*                        解析协议导出                                  */
/************************************************************************/
extern "C" BOOL P2XPProtocol_Parse_Login(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPPEER_PROTOCOL * pSt_P2XPPeer)
{
	return m_P2XPParse.P2XPProtocol_Parse_Login(lpszMsgBuffer, nMsgLen, pSt_P2XPPeer);
}
extern "C" BOOL P2XPProtocol_Parse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszPubAddr, TCHAR * ptszPriAddr)
{
	return m_P2XPParse.P2XPProtocol_Parse_List(lpszMsgBuffer, nMsgLen, ptszPubAddr, ptszPriAddr);
}
extern "C" BOOL P2XPProtocol_Parse_User(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszUserName)
{
	return m_P2XPParse.P2XPProtocol_Parse_User(lpszMsgBuffer, nMsgLen, ptszUserName);
}
extern "C" BOOL P2XPProtocol_Parse_Connect(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPIO_PROTOCOL * pSt_IOProtocol)
{
	return m_P2XPParse.P2XPProtocol_Parse_Connect(lpszMsgBuffer, nMsgLen, pSt_IOProtocol);
}
/************************************************************************/
/*                        打包协议导出                                  */
/************************************************************************/
extern "C" BOOL P2XPProtocol_Packet_Common(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCTSTR lpszMsgBuffer)
{
	return m_P2XPPacket.P2XPProtocol_Packet_Common(pSt_ProtocolHdr, ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer);
}
extern "C" BOOL P2XPProtocol_Packet_Lan(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * **pppSt_ListClients, int nListCount, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_P2XPPacket.P2XPProtocol_Packet_Lan(pSt_ProtocolHdr, pppSt_ListClients, nListCount, ptszMsgBuffer, pInt_Len);
}
extern "C" BOOL P2XPProtocol_Packet_WLan(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, list<XENGINE_P2XPPEER_PROTOCOL>*pStl_ListClients, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_P2XPPacket.P2XPProtocol_Packet_WLan(pSt_ProtocolHdr, pStl_ListClients, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL P2XPProtocol_Packet_User(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * pSt_PeerInfo, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_P2XPPacket.P2XPProtocol_Packet_User(pSt_ProtocolHdr, pSt_PeerInfo, ptszMsgBuffer, pInt_Len);
}
extern "C" BOOL P2XPProtocol_Packet_Connect(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPIO_PROTOCOL * pSt_IOProtocol, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_P2XPPacket.P2XPProtocol_Packet_Connect(pSt_ProtocolHdr, pSt_IOProtocol, ptszMsgBuffer, pInt_MsgLen);
}