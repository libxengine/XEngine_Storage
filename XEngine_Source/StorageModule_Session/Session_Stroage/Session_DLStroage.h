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
    bool Session_DLStroage_Init(int nMaxConnect);
    bool Session_DLStroage_Destory();
    bool Session_DLStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x* pInt_Count, __int64x* pInt_LeftCount, int nPosStart = 0, int nPostEnd = 0, LPCXSTR lpszFileHash = NULL, int nLimit = 0, XHANDLE xhToken = NULL);
    bool Session_DLStroage_GetBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    bool Session_DLStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
    bool Session_DLStroage_GetCount(int* pInt_ListCount);
    bool Session_DLStorage_SetSeek(LPCXSTR lpszClientAddr, int nSeek);
    bool Session_DLStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
    bool Session_DLStroage_Delete(LPCXSTR lpszClientAddr);
    bool Session_DLStroage_MaxConnect(LPCXSTR lpszClientAddr);
private:
    int m_nMaxConnect;
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_STORAGEINFO> stl_MapStroage;
};