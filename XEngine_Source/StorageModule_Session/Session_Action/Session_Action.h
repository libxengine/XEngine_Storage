#pragma once
/********************************************************************
//    Created:     2024/05/10  14:51:09
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Action\Session_Action.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Action
//    File Base:   Session_Action
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     转录动作会话类
//    History:
*********************************************************************/
typedef struct  
{
    XENGINE_ACTIONINFO st_ActionInfo;
    XHANDLE xhAction;
}SESSION_ACTIONINFO;

class CSession_Action
{
public:
    CSession_Action();
    ~CSession_Action();
public:
    bool Session_Action_Insert(XNETHANDLE xhToken, XHANDLE xhAction, XENGINE_ACTIONINFO* pSt_ActionInfo);
    XHANDLE Session_Action_GetToken(XNETHANDLE xhToken);
    bool Session_Action_GetInfo(XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo);
    bool Session_Action_Delete(XNETHANDLE xhToken);
private:
    shared_mutex st_Locker;
private:
    unordered_map<XNETHANDLE, SESSION_ACTIONINFO> stl_MapAction;
};