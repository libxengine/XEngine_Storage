#pragma once
/********************************************************************
//    Created:     2021/06/02  14:10:18
//    File Name:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage\Session_DLStroage.h
//    File Path:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage
//    File Base:   Session_DLStroage
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储下载会话
//    History:
*********************************************************************/
#define XENGINE_STOREAGE_SESSION_DOWNLOAD_SENDTIME 10

typedef struct
{
    shared_ptr<shared_mutex> st_Locker;
    list<SESSION_STORAGEINFO>* pStl_ListStorage;
}SESSION_STORAGELIST;

class CSession_DLStroage
{
public:
    CSession_DLStroage();
    ~CSession_DLStroage();
public:
    BOOL Session_DLStroage_Init(int nPoolCount = 1, int nTryTime = 3, int nAutoSpeed = 3);
    BOOL Session_DLStroage_Destory();
    BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x* pInt_Count, __int64x* pInt_LeftCount, int nPosStart = 0, int nPostEnd = 0);
    BOOL Session_DLStroage_GetBuffer(int nPool, LPCTSTR lpszClientAddr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    BOOL Session_DLStroage_GetInfo(int nPool, LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
    BOOL Session_DLStroage_GetCount(int nPool, list<string>* pStl_ListClient);
    BOOL Session_DLStorage_SetSeek(LPCTSTR lpszClientAddr, int nSeek, BOOL bError = TRUE, SESSION_STORAGEDYNAMICRATE* pSt_StorageRate = NULL);
    BOOL Session_DLStorage_GetAll(int nPool, SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
    BOOL Session_DLStroage_Delete(LPCTSTR lpszClientAddr);
private:
    int m_nTryTime;
    int m_nTryAuto;
    shared_mutex st_Locker;
private:
    unordered_map<int, SESSION_STORAGELIST> stl_MapStroage;
};