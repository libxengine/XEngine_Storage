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
    bool Session_UPStroage_Init(int nMaxConnect, bool bUPResume = false);
    bool Session_UPStroage_Destory();
    bool Session_UPStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x nFileSize, bool bRewrite, int nPosStart = 0, int nPostEnd = 0);
    bool Session_UPStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO* pSt_StorageInfo);
    bool Session_UPStroage_Write(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
    bool Session_UPStroage_Exist(LPCXSTR lpszClientAddr);
    bool Session_UPStorage_GetAll(SESSION_STORAGEINFO*** pppSt_StorageInfo, int* pInt_ListCount);
    bool Session_UPStroage_Delete(LPCXSTR lpszClientAddr);
    bool Session_UPStroage_Close(LPCXSTR lpszClientAddr);
    bool Session_UPStroage_MaxConnect(LPCXSTR lpszClientAddr);
    bool Session_UPStroage_SetBoundary(LPCXSTR lpszClientAddr, LPCXSTR lpszBoundary);
    bool Session_UPStroage_SetBoundaryStart(LPCXSTR lpszClientAddr);
private:
    bool m_bResume;
    int m_nMaxConnect;
private:
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_STORAGEUPLOADER> stl_MapStroage;
};
