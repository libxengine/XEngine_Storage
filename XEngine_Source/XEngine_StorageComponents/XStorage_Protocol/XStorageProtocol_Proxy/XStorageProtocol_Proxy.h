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
    BOOL XStorageProtocol_Proxy_PacketUPDown(LPCTSTR lpszFileName, LPCTSTR lpszClientAddr, __int64x nFileSize, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileHash = NULL);
    BOOL XStorageProtocol_Proxy_ParseNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszClientAddr, TCHAR* ptszFileName, TCHAR* ptszFileHash, __int64x* pInt_FileSize);
protected:
private:
};
