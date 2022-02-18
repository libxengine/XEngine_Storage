#pragma once
/********************************************************************
//    Created:     2021/06/18  14:59:38
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_User\Session_User.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_User
//    File Base:   Session_User
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储用户管理
//    History:
*********************************************************************/

typedef struct
{
    TCHAR tszUserName[128];
    TCHAR tszUserPass[128];
}SESSION_USERINFO;

class CSession_User
{
public:
    CSession_User();
    ~CSession_User();
public:
    BOOL Session_User_Init(LPCTSTR lpszUserFile);
	BOOL Session_User_Destory();
    BOOL Session_User_Exist(LPCTSTR lpszUser, LPCTSTR lpszPass);
private:
    shared_mutex st_Locker;
private:
    unordered_map<string, SESSION_USERINFO> stl_MapUser;
};
