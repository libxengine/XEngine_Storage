#pragma once
/********************************************************************
//    Created:     2021/10/25  15:22:08
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Parse\Protocol_StorageParse.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\Protocol_Parse
//    File Base:   Protocol_StorageParse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储服务协议解析类
//    History:
*********************************************************************/

class CProtocol_StorageParse
{
public:
    CProtocol_StorageParse();
    ~CProtocol_StorageParse();
public:
    bool Protocol_StorageParse_QueryFile(LPCXSTR lpszMsgBuffer, XCHAR* ptszTimeStart, XCHAR* ptszTimeEnd, XCHAR* ptszBuckKey = NULL, XCHAR* ptszFileName = NULL, XCHAR* ptszFileHash = NULL, int* pInt_Mode = NULL, XNETHANDLE* pxhToken = NULL);
    bool Protocol_StorageParse_ReportFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int* pInt_ListCount);
	bool Protocol_StorageParse_DirOperator(LPCXSTR lpszMsgBuffer, XCHAR* ptszUserDir, XCHAR* ptszBuckKey, int* pInt_Operator);
    bool Protocol_StorageParse_ProxyNotify(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszClientAddr, XCHAR* ptszBuckKey, XCHAR* ptszFileName, XCHAR* ptszFileHash, __int64x* pInt_FileSize);
    bool Protocol_StorageParse_SpeedLimit(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Code, int* pInt_Limit);
    bool Protocol_StorageParse_P2PToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE* pxhToken);
    bool Protocol_StorageParse_Action(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_ACTIONINFO* pSt_ActionInfo);
public:
    bool Protocol_StorageParse_WDLock(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock);
    bool Protocol_StorageParse_WDPropPatch(LPCXSTR lpszMsgBuffer, int nMsgLen, std::list<string>* pStl_ListName);
};
