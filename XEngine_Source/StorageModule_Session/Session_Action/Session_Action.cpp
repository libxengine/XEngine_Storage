#include "pch.h"
#include "Session_Action.h"
/********************************************************************
//    Created:     2024/05/10  14:57:36
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Action\Session_Action.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Action
//    File Base:   Session_Action
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     转录动作会话类
//    History:
*********************************************************************/
CSession_Action::CSession_Action()
{
}
CSession_Action::~CSession_Action()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Action_Insert
函数功能：插入一个会话到动作管理器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要插入的句柄
 参数.二：xhAction
  In/Out：In
  类型：句柄
  可空：N
  意思：输入下载器句柄
 参数.三：pSt_ActionInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Action::Session_Action_Insert(XNETHANDLE xhToken, XHANDLE xhAction, XENGINE_ACTIONINFO* pSt_ActionInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == xhAction) || (NULL == pSt_ActionInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}
	SESSION_ACTIONINFO st_ActionInfo = {};
	st_ActionInfo.xhAction = xhAction;
	st_ActionInfo.st_ActionInfo = *pSt_ActionInfo;

	st_Locker.lock();
	stl_MapAction.insert(make_pair(xhToken, st_ActionInfo));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_Action_GetToken
函数功能：获得一个下载器句柄
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
返回值
  类型：句柄型
  意思：返回下载器句柄
备注：
*********************************************************************/
XHANDLE CSession_Action::Session_Action_GetToken(XNETHANDLE xhToken)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, SESSION_ACTIONINFO>::iterator stl_MapIterator = stl_MapAction.find(xhToken);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return NULL;
	}
	XHANDLE xhAction = stl_MapIterator->second.xhAction;
	st_Locker.unlock_shared();
	return xhAction;
}
/********************************************************************
函数名称：Session_Action_GetInfo
函数功能：获取下载信息
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
 参数.二：pSt_ActionInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Action::Session_Action_GetInfo(XNETHANDLE xhToken, XENGINE_ACTIONINFO* pSt_ActionInfo)
{
	Session_IsErrorOccur = false;

	if ((NULL == pSt_ActionInfo))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PARAMENT;
		return false;
	}

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, SESSION_ACTIONINFO>::iterator stl_MapIterator = stl_MapAction.find(xhToken);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pSt_ActionInfo = stl_MapIterator->second.st_ActionInfo;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_Action_Delete
函数功能：删除一个动作管理器
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的动作句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Action::Session_Action_Delete(XNETHANDLE xhToken)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	unordered_map<XNETHANDLE, SESSION_ACTIONINFO>::iterator stl_MapIterator = stl_MapAction.find(xhToken);
	if (stl_MapIterator != stl_MapAction.end())
	{
		stl_MapAction.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}