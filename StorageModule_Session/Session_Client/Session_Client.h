#pragma once
/********************************************************************
//    Created:     2021/05/08  13:52:29
//    File Name:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session\Session_Client\Session_Client.h
//    File Path:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session\Session_Client
//    File Base:   Session_Client
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端会话类
//    History:
*********************************************************************/
typedef struct  
{
    XENGINE_PROTOCOL_USERREG st_UserInfo;
}SESSION_CLIENT;


class CSession_Client
{
public:
    CSession_Client();
    ~CSession_Client();
public:
	BOOL Session_Client_Insert(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser);
    BOOL Session_Client_Get(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser);
    BOOL Session_Client_Delete(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser = NULL);
private:
    shared_mutex st_Locker;
private:
    unordered_map<tstring, SESSION_CLIENT> stl_MapClient;
};
