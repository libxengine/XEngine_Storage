#pragma once
/********************************************************************
//    Created:     2021/10/25  14:41:18
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet\Protocol_StoragePacket.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Packet
//    File Base:   Protocol_StoragePacket
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储服务协议打包类
//    History:
*********************************************************************/

class CProtocol_StoragePacket
{
public:
    CProtocol_StoragePacket();
    ~CProtocol_StoragePacket();
public:
    bool Protocol_StoragePacket_QueryFile(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL, XNETHANDLE xhToken = 0);
    bool Protocol_StoragePacket_Info(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO*** pppSt_DLInfo, SESSION_STORAGEINFO*** pppSt_UPInfo, int nDLCount, int nUPCount);
	bool Protocol_StoragePacket_DirOperator(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** pppszListEnum, int nListCount);
	bool Protocol_StoragePacket_BasicAuth(LPCXSTR lpszMethod, LPCXSTR lpszPostUrl, LPCXSTR lpszClientAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    bool Protocol_StoragePacket_UPDown(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszBuckKey, LPCXSTR lpszFileName, LPCXSTR lpszClientAddr, __int64x nFileSize, bool bDown, LPCXSTR lpszFileHash = NULL);
    bool Protocol_StoragePacket_REQFile(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL, XNETHANDLE xhToken = 0);
    bool Protocol_StoragePacket_Action(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo);
public:
    bool Protocol_StoragePacket_WDPropfind(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** ppptszListFile, int nFileCount, LPCXSTR lpszBucketPath, LPCXSTR lpszBucketKey);
    bool Protocol_StoragePacket_WDLock(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock);
    bool Protocol_StoragePacket_WDPropPatch(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName, std::list<string>* pStl_ListName);
};
