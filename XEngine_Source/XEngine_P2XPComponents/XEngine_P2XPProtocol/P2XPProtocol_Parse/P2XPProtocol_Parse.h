#pragma once
/********************************************************************
//    Created:     2021/07/19  09:35:48
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Parse\P2XPProtocol_Parse.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_P2XPComponents\XEngine_P2XPProtocol\P2XPProtocol_Parse
//    File Base:   P2XPProtocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     解析类
//    History:
*********************************************************************/

class CP2XPProtocol_Parse
{
public:
    CP2XPProtocol_Parse();
    ~CP2XPProtocol_Parse();
public:
    BOOL P2XPProtocol_Parse_Login(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPPEER_PROTOCOL* pSt_P2XPPeer);
    BOOL P2XPProtocol_Parse_List(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszPubAddr, TCHAR* ptszPriAddr);
    BOOL P2XPProtocol_Parse_User(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszUserName);
    BOOL P2XPProtocol_Parse_Connect(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_P2XPIO_PROTOCOL* pSt_IOProtocol);
protected:
private:
};
