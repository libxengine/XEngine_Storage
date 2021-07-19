/********************************************************************
//    Created:     2021/07/19  10:22:02
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Packet\P2XPProtocol_Packet.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Packet
//    File Base:   P2XPProtocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     打包类
//    History:
*********************************************************************/

class CP2XPProtocol_Packet
{
public:
	CP2XPProtocol_Packet();
	~CP2XPProtocol_Packet();
public:
	BOOL P2XPProtocol_Packet_Common(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL);
	BOOL P2XPProtocol_Packet_Lan(XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR* ptszMsgBuffer, int* pInt_Len);
	BOOL P2XPProtocol_Packet_User(XENGINE_P2XPPEER_PROTOCOL* pSt_PeerInfo, TCHAR* ptszMsgBuffer, int* pInt_Len);
protected:
private:
};