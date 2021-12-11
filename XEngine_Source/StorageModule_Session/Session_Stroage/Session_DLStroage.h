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

class CSession_DLStroage
{
public:
    CSession_DLStroage();
    ~CSession_DLStroage();
public:
    BOOL Session_DLStroage_Init(int nTryTime);
    BOOL Session_DLStroage_Destory();
    BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x* pInt_Count, __int64x* pInt_LeftCount, int nPosStart = 0, int nPostEnd = 0, LPCTSTR lpszFileHash = NULL);
    BOOL Session_DLStroage_GetBuffer(LPCTSTR lpszClientAddr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    BOOL Session_DLStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
    BOOL Session_DLStroage_GetCount(int* pInt_ListCount);
    BOOL Session_DLStorage_SetSeek(LPCTSTR lpszClientAddr, int nSeek, BOOL bError = TRUE);
    BOOL Session_DLStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
    BOOL Session_DLStroage_Delete(LPCTSTR lpszClientAddr);
private:
    int m_nTryTime;
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_STORAGEINFO> stl_MapStroage;
};