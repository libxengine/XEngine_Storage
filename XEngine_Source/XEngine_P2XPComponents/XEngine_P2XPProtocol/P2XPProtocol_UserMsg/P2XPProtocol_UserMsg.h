#pragma once
/********************************************************************
//	Created:	2013/1/15  18:03
//	File Name: 	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_UserMsg\P2XPProtocol_UserMsg.h
//	File Path:	G:\U_DISK_Path\NetSocketEngine\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_UserMsg
//	File Base:	P2XPProtocol_UserMsg
//	File Ext:	h
//  Project:    NetSocketEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	P2XP协议管理器，用户消息协议
//	History:
*********************************************************************/

class CP2XPProtocol_UserMsg
{
public:
    CP2XPProtocol_UserMsg();
    ~CP2XPProtocol_UserMsg();
public:
    BOOL P2XPProtocol_UserMsg_RequestLogin(XENGINE_P2XPPEER_PROTOCOL *pSt_ClientInfo, XENGINE_PROTOCOL_USERAUTH *pSt_UserLogin, TCHAR *ptszMsgBuffer, int *pInt_Len, WORD wPacketSerial = 0);
    BOOL P2XPProtocol_UserMsg_RequestConnect(XENGINE_P2XPIO_PROTOCOL*pSt_ClientIONet, TCHAR *ptszMsgBuffer, int *pInt_Len, WORD wPacketSerial = 0);
    BOOL P2XPProtocol_UserMsg_RequestLan(TCHAR *ptszMsgBuffer, int *pInt_Len, BOOL bWlan = FALSE, WORD wPacketSerial = 0);
    BOOL P2XPProtocol_UserMsg_RequestHeartBeat(TCHAR *ptszMsgBuffer, int *pInt_Len);
protected:
private:
};
