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
    BOOL Protocol_StorageParse_QueryFile(LPCTSTR lpszMsgBuffer, TCHAR* ptszTimeStart, TCHAR* ptszTimeEnd, TCHAR* ptszPathKey = NULL, TCHAR* ptszFileName = NULL, TCHAR* ptszFileHash = NULL);
    BOOL Protocol_StorageParse_ReportFile(LPCTSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int* pInt_ListCount);
	BOOL Protocol_StorageParse_DirOperator(LPCTSTR lpszMsgBuffer, TCHAR* ptszUserDir, TCHAR* ptszBuckKey, int* pInt_Operator);
    BOOL Protocol_StorageParse_ProxyNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszClientAddr, TCHAR* ptszPathKey, TCHAR* ptszFileName, TCHAR* ptszFileHash, __int64x* pInt_FileSize);
};
