#pragma once
/********************************************************************
//    Created:     2021/06/03  11:39:54
//    File Name:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage\Session_UPStroage.h
//    File Path:   D:\XEngine_Storage\StorageModule_Session\Session_Stroage
//    File Base:   Session_UPStroage
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储上传会话
//    History:
*********************************************************************/
typedef struct
{
    shared_ptr<shared_mutex> st_Locker;
    list<SESSION_STORAGEINFO>* pStl_ListStorage;
}SESSION_STORAGELIST;

class CSession_UPStroage
{
public:
    CSession_UPStroage();
    ~CSession_UPStroage();
public:
    BOOL Session_UPStroage_Init(int nPoolCount = 1);
    BOOL Session_UPStroage_Destory();
    BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x* pInt_Count = NULL, int nPos = 0);
    BOOL Session_UPStroage_GetList(int nPool, int nIndex, TCHAR* ptszClientAddr, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
    BOOL Session_UPStroage_GetCount(int nPool, int* pInt_ListCount);
    BOOL Session_UPStroage_Delete(LPCTSTR lpszClientAddr);
private:
    shared_mutex st_Locker;
private:
    unordered_map<int, SESSION_STORAGELIST> stl_MapStroage;
};
