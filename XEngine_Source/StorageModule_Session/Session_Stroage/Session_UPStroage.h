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
    time_t nTimeStart;
    SESSION_STORAGEINFO st_StorageInfo;
}SESSION_STORAGEUPLOADER;

class CSession_UPStroage
{
public:
    CSession_UPStroage();
    ~CSession_UPStroage();
public:
    BOOL Session_UPStroage_Init(BOOL bUPResume = FALSE);
    BOOL Session_UPStroage_Destory();
    BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszBuckKey, LPCTSTR lpszFileDir, __int64x nFileSize, __int64x nLeftCount, int nPosStart = 0, int nPostEnd = 0);
    BOOL Session_UPStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
    BOOL Session_UPStroage_Write(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);
    BOOL Session_UPStroage_Exist(LPCTSTR lpszClientAddr);
    BOOL Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
    BOOL Session_UPStroage_Delete(LPCTSTR lpszClientAddr);
    BOOL Session_UPStroage_Close(LPCTSTR lpszClientAddr);
private:
    BOOL m_bResume;
private:
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_STORAGEUPLOADER> stl_MapStroage;
};
