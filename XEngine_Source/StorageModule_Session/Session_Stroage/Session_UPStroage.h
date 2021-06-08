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
    __int64x nWriteLen;
    time_t nTimeStart;
    SESSION_STORAGEINFO st_StorageInfo;
}SESSION_STORAGEUPLOADER;

class CSession_UPStroage
{
public:
    CSession_UPStroage();
    ~CSession_UPStroage();
public:
    BOOL Session_UPStroage_Init();
    BOOL Session_UPStroage_Destory();
    BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x nFileSize, int nPos = 0);
    BOOL Session_UPStroage_GetComplete(LPCTSTR lpszClientAddr, BOOL* pbComplete);
    BOOL Session_UPStroage_Write(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);
    BOOL Session_UPStroage_Exist(LPCTSTR lpszClientAddr);
    BOOL Session_UPStroage_Delete(LPCTSTR lpszClientAddr);
private:
    shared_mutex st_Locker;
private:
    unordered_map<tstring, SESSION_STORAGEUPLOADER> stl_MapStroage;
};
