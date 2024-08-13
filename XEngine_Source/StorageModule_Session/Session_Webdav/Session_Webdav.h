#pragma once
/********************************************************************
//    Created:     2024/08/05  16:14:56
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Webdav\Session_Webdav.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Webdav
//    File Base:   Session_Webdav
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     WEBDAV会话
//    History:
*********************************************************************/
typedef struct  
{
    XENGINE_WEBDAVLOCK st_WDLocker;
    time_t nTimeStart;
}SESSION_WEBDAV;

class CSession_Webdav
{
public:
    CSession_Webdav();
    ~CSession_Webdav();
public:
    bool Session_Webdav_Insert(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker, int nTimeout = 3600);
    bool Session_Webdav_Get(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker);
    bool Session_Webdav_Delete(LPCXSTR lpszFileName);
private:
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_WEBDAV> stl_MapAction;
};