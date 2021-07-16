#pragma once
/********************************************************************
//	Created:	2018/11/7   10:55
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_ServiceMsg\P2XPProtocol_ServiceMsg.h
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_P2xp\NetEngine_P2XPProtocol\P2XPProtocol_ServiceMsg
//	File Base:	P2XPProtocol_ServiceMsg
//	File Ext:	h
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	服务器消息协议
//	History:
*********************************************************************/

class CP2XPProtocol_ServiceMsg
{
public:
    CP2XPProtocol_ServiceMsg();
    ~CP2XPProtocol_ServiceMsg();
public:
    BOOL P2XPProtocol_ServiceMsg_ResponseJson(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, TCHAR *ptszMsgBuffer, int *pInt_Len, int nCode = 0, LPCTSTR lpszCodeMsg = NULL);
    BOOL P2XPProtocol_ServiceMsg_ResponseLan(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL*** pppSt_ListClients, int nListCount, TCHAR *ptszMsgBuffer, int *pInt_Len);
    BOOL P2XPProtocol_ServiceMsg_ResponseQueryUser(XENGINE_PROTOCOLHDR *pSt_ProtocolHdr, XENGINE_P2XPPEER_PROTOCOL*pSt_PeerInfo, TCHAR *ptszMsgBuffer, int *pInt_Len);
protected:
private:
};
