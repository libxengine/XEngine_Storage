#include "pch.h"
#include "Session_Webdav.h"
/********************************************************************
//    Created:     2024/08/05  16:16:27
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Webdav\Session_Webdav.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_Webdav
//    File Base:   Session_Webdav
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     WEBDAV会话
//    History:
*********************************************************************/
CSession_Webdav::CSession_Webdav()
{
}
CSession_Webdav::~CSession_Webdav()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_Webdav_Insert
函数功能：插入一个会话
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的名称
 参数.二：pSt_WDLocker
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入插入的内容
 参数.三：nTimeout
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入超时时间
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Webdav::Session_Webdav_Insert(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker, int nTimeout)
{
	Session_IsErrorOccur = false;

	SESSION_WEBDAV st_WDSession = {};
	
	_xstprintf(pSt_WDLocker->tszTimeout, _X("Second-%d"), nTimeout);
	int nRet = _xstprintf(pSt_WDLocker->tszToken, _X("opaquelocktoken:"));
	BaseLib_OperatorHandle_CreateGuid(pSt_WDLocker->tszToken + nRet);

	st_WDSession.nTimeStart = time(NULL);
	st_WDSession.st_WDLocker = *pSt_WDLocker;

	st_Locker.lock();
	stl_MapAction.insert(make_pair(lpszFileName, st_WDSession));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_Webdav_Get
函数功能：获取信息
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
 参数.二：pSt_WDLocker
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Webdav::Session_Webdav_Get(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<string, SESSION_WEBDAV>::iterator stl_MapIterator = stl_MapAction.find(lpszFileName);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return NULL;
	}
	*pSt_WDLocker = stl_MapIterator->second.st_WDLocker;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：Session_Webdav_Delete
函数功能：删除一个会话
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_Webdav::Session_Webdav_Delete(LPCXSTR lpszFileName)
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	unordered_map<string, SESSION_WEBDAV>::iterator stl_MapIterator = stl_MapAction.find(lpszFileName);
	if (stl_MapIterator != stl_MapAction.end())
	{
		stl_MapAction.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}