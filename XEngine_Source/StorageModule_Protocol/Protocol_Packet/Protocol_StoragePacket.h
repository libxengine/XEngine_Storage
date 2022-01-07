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
    BOOL Protocol_StoragePacket_QueryFile(TCHAR *ptszMsgBuffer, int *pInt_MsgLen, XSTORAGECORE_DBFILE*** pppSt_DBFile, int nListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
    BOOL Protocol_StoragePacket_Info(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO*** pppSt_DLInfo, SESSION_STORAGEINFO*** pppSt_UPInfo, int nDLCount, int nUPCount);
	BOOL Protocol_StoragePacket_DirOperator(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** pppszListEnum, int nListCount);
	BOOL Protocol_StoragePacket_BasicAuth(LPCTSTR lpszMethod, LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    BOOL Protocol_StoragePacket_UPDown(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszBuckKey, LPCTSTR lpszFileName, LPCTSTR lpszClientAddr, __int64x nFileSize, LPCTSTR lpszFileHash = NULL);
};
