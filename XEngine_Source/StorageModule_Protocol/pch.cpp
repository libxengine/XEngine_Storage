#include "pch.h"
#include "Protocol_Parse/Protocol_P2XPParse.h"
#include "Protocol_Parse/Protocol_StorageParse.h"
#include "Protocol_Packet/Protocol_P2XPPacket.h"
#include "Protocol_Packet/Protocol_StoragePacket.h"
/********************************************************************
//    Created:     2021/10/25  14:38:54
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL Protocol_IsErrorOccur = FALSE;
DWORD Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_P2XPParse m_P2XPParse;
CProtocol_P2XPPacket m_P2XPPacket;
CProtocol_StoragePacket m_StoragePacket;
CProtocol_StorageParse m_StorageParse;
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Protocol_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                        解析协议导出                                  */
/************************************************************************/
extern "C" BOOL Protocol_P2XPParse_Login(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPPEER_PROTOCOL * pSt_P2XPPeer)
{
	return m_P2XPParse.Protocol_P2XPParse_Login(lpszMsgBuffer, nMsgLen, pSt_P2XPPeer);
}
extern "C" BOOL Protocol_P2XPParse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszPubAddr, TCHAR * ptszPriAddr)
{
	return m_P2XPParse.Protocol_P2XPParse_List(lpszMsgBuffer, nMsgLen, ptszPubAddr, ptszPriAddr);
}
extern "C" BOOL Protocol_P2XPParse_User(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszUserName)
{
	return m_P2XPParse.Protocol_P2XPParse_User(lpszMsgBuffer, nMsgLen, ptszUserName);
}
extern "C" BOOL Protocol_P2XPParse_Connect(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPIO_PROTOCOL * pSt_IOProtocol)
{
	return m_P2XPParse.Protocol_P2XPParse_Connect(lpszMsgBuffer, nMsgLen, pSt_IOProtocol);
}
//////////////////////////////////////////////////////////////////////////
extern "C" BOOL Protocol_StorageParse_ProxyNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszClientAddr, TCHAR * ptszBuckKey, TCHAR * ptszFileName, TCHAR * ptszFileHash, __int64x * pInt_FileSize)
{
	return m_StorageParse.Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, ptszClientAddr, ptszBuckKey, ptszFileName, ptszFileHash, pInt_FileSize);
}
extern "C" BOOL Protocol_StorageParse_QueryFile(LPCTSTR lpszMsgBuffer, TCHAR * ptszTimeStart, TCHAR * ptszTimeEnd, CHAR * ptszBuckKey, TCHAR * ptszFileName, TCHAR * ptszFileHash, int* pInt_Mode)
{
	return m_StorageParse.Protocol_StorageParse_QueryFile(lpszMsgBuffer, ptszTimeStart, ptszTimeEnd, ptszBuckKey, ptszFileName, ptszFileHash, pInt_Mode);
}
extern "C" BOOL Protocol_StorageParse_ReportFile(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int* pInt_ListCount)
{
	return m_StorageParse.Protocol_StorageParse_ReportFile(lpszMsgBuffer, nMsgLen, pppSt_DBFile, pInt_ListCount);
}
extern "C" BOOL Protocol_StorageParse_DirOperator(LPCTSTR lpszMsgBuffer, TCHAR * ptszUserDir, TCHAR * ptszBuckKey, int* pInt_Operator)
{
	return m_StorageParse.Protocol_StorageParse_DirOperator(lpszMsgBuffer, ptszUserDir, ptszBuckKey, pInt_Operator);
}
/************************************************************************/
/*                        打包协议导出                                  */
/************************************************************************/
extern "C" BOOL Protocol_P2XPPacket_Common(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCTSTR lpszMsgBuffer)
{
	return m_P2XPPacket.Protocol_P2XPPacket_Common(pSt_ProtocolHdr, ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer);
}
extern "C" BOOL Protocol_P2XPPacket_Lan(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * **pppSt_ListClients, int nListCount, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_P2XPPacket.Protocol_P2XPPacket_Lan(pSt_ProtocolHdr, pppSt_ListClients, nListCount, ptszMsgBuffer, pInt_Len);
}
extern "C" BOOL Protocol_P2XPPacket_WLan(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, list<XENGINE_P2XPPEER_PROTOCOL>*pStl_ListClients, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_P2XPPacket.Protocol_P2XPPacket_WLan(pSt_ProtocolHdr, pStl_ListClients, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Protocol_P2XPPacket_User(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL * pSt_PeerInfo, XENGINE_IPADDRINFO * pSt_AddrInfo, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	return m_P2XPPacket.Protocol_P2XPPacket_User(pSt_ProtocolHdr, pSt_PeerInfo, pSt_AddrInfo, ptszMsgBuffer, pInt_Len);
}
extern "C" BOOL Protocol_P2XPPacket_Connect(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, XENGINE_P2XPIO_PROTOCOL * pSt_IOProtocol, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_P2XPPacket.Protocol_P2XPPacket_Connect(pSt_ProtocolHdr, pSt_IOProtocol, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Protocol_P2XPPacket_QueryFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileName, LPCTSTR lpszFileHash)
{
	return m_P2XPPacket.Protocol_P2XPPacket_QueryFile(ptszMsgBuffer, pInt_MsgLen, lpszFileName, lpszFileHash);
}
//////////////////////////////////////////////////////////////////////////
extern "C" BOOL Protocol_StoragePacket_BasicAuth(LPCTSTR lpszMethod, LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_StoragePacket.Protocol_StoragePacket_BasicAuth(lpszMethod, lpszPostUrl, lpszClientAddr, lpszUser, lpszPass, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Protocol_StoragePacket_UPDown(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszBuckKey, LPCTSTR lpszFileName, LPCTSTR lpszClientAddr, __int64x nFileSize, BOOL bDown, LPCTSTR lpszFileHash)
{
	return m_StoragePacket.Protocol_StoragePacket_UPDown(ptszMsgBuffer, pInt_MsgLen, lpszBuckKey, lpszFileName, lpszClientAddr, nFileSize, bDown, lpszFileHash);
}
extern "C" BOOL Protocol_StoragePacket_QueryFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int nListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
	return m_StoragePacket.Protocol_StoragePacket_QueryFile(ptszMsgBuffer, pInt_MsgLen, pppSt_DBFile, nListCount, lpszTimeStart, lpszTimeEnd);
}
extern "C" BOOL Protocol_StoragePacket_Info(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO * **pppSt_DLInfo, SESSION_STORAGEINFO * **pppSt_UPInfo, int nDLCount, int nUPCount)
{
	return m_StoragePacket.Protocol_StoragePacket_Info(ptszMsgBuffer, pInt_MsgLen, pppSt_DLInfo, pppSt_UPInfo, nDLCount, nUPCount);
}
extern "C" BOOL Protocol_StoragePacket_DirOperator(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, TCHAR * **pppszListEnum, int nListCount)
{
	return m_StoragePacket.Protocol_StoragePacket_DirOperator(ptszMsgBuffer, pInt_MsgLen, pppszListEnum, nListCount);
}