#pragma once
/********************************************************************
//    Created:     2021/06/21  10:22:40
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_Protocol\XStorageProtocol_Proxy\XStorageProtocol_Proxy.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_Protocol\XStorageProtocol_Proxy
//    File Base:   XStorageProtocol_Proxy
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理转发类
//    History:
*********************************************************************/

class CXStorageProtocol_Proxy
{
public:
    CXStorageProtocol_Proxy();
    ~CXStorageProtocol_Proxy();
public:
    BOOL XStorageProtocol_Proxy_PacketBasicAuth(LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
protected:
private:
};
