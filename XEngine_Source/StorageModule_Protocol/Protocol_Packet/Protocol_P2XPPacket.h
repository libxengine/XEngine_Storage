#pragma once
/********************************************************************
//    Created:     2021/10/25  14:34:34
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet\Protocol_P2XPPacket.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet
//    File Base:   Protocol_P2XPPacket
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     打包类
//    History:
*********************************************************************/
typedef struct  
{
	TCHAR tszUsername[MAX_PATH];
	TCHAR tszClientAddr[128];
}P2XPPROTOCOL_LANPACKET;

class CProtocol_P2XPPacket
{
public:
	CProtocol_P2XPPacket();
	~CProtocol_P2XPPacket();
public:
	BOOL Protocol_P2XPPacket_Common(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL);
	BOOL Protocol_P2XPPacket_Lan(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	BOOL Protocol_P2XPPacket_WLan(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, list<XENGINE_P2XPPEER_PROTOCOL>* pStl_ListClients, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	BOOL Protocol_P2XPPacket_User(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL* pSt_PeerInfo, XENGINE_IPADDRINFO* pSt_AddrInfo, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	BOOL Protocol_P2XPPacket_Connect(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, XENGINE_P2XPIO_PROTOCOL* pSt_IOProtocol, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	BOOL Protocol_P2XPPacket_QueryFile(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
protected:
private:
};