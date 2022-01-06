#pragma once
/********************************************************************
//    Created:     2021/10/25  14:32:41
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Parse\Protocol_P2XPParse.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Parse
//    File Base:   Protocol_P2XPParse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     解析类
//    History:
*********************************************************************/

class CProtocol_P2XPParse
{
public:
    CProtocol_P2XPParse();
    ~CProtocol_P2XPParse();
public:
    BOOL Protocol_P2XPParse_Login(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPPEER_PROTOCOL* pSt_P2XPPeer);
    BOOL Protocol_P2XPParse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszPubAddr, TCHAR* ptszPriAddr);
    BOOL Protocol_P2XPParse_User(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszUserName);
    BOOL Protocol_P2XPParse_Connect(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPIO_PROTOCOL* pSt_IOProtocol);
protected:
private:
};
