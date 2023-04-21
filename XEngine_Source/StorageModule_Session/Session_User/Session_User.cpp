#include "pch.h"
#include "Session_User.h"
/********************************************************************
//    Created:     2021/06/18  15:01:25
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_User\Session_User.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_User
//    File Base:   Session_User
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     存储用户管理
//    History:
*********************************************************************/
CSession_User::CSession_User()
{
}
CSession_User::~CSession_User()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_User_Init
函数功能：初始化用户管理器
 参数.一：lpszUserFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户文件地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_User::Session_User_Init(LPCXSTR lpszUserFile)
{
	Session_IsErrorOccur = false;

	FILE* pSt_File = _xtfopen(lpszUserFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_OPENFILE;
		return false;
	}
	int nCount = 0;
	XCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	while (true)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 1024, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);
	
	LPCXSTR lpszLineStr = _X("\r\n");
	XCHAR* ptszTokStr = _tcsxtok(tszMsgBuffer, lpszLineStr);
	while (1)
	{
		if (NULL == ptszTokStr)
		{
			break;
		}
		SESSION_USERINFO st_UserInfo;
		memset(&st_UserInfo, '\0', sizeof(SESSION_USERINFO));

		_stxscanf(ptszTokStr, _X("%s %s %s"), st_UserInfo.tszUserName, st_UserInfo.tszUserPass, st_UserInfo.tszUserLimit);
		stl_MapUser.insert(make_pair(st_UserInfo.tszUserName, st_UserInfo));

		ptszTokStr = _tcsxtok(NULL, lpszLineStr);
	}
	return true;
}
/********************************************************************
函数名称：Session_User_Destory
函数功能：销毁管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_User::Session_User_Destory()
{
	Session_IsErrorOccur = false;

	st_Locker.lock();
	stl_MapUser.clear();
	st_Locker.unlock();

	return true;
}
/********************************************************************
函数名称：Session_User_Exist
函数功能：用户名和密码是否存在
 参数.一：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要判断的用户名
 参数.二：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要判断的密码
 参数.三：pInt_Limit
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出获取到的限速
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_User::Session_User_Exist(LPCXSTR lpszUser, LPCXSTR lpszPass, int* pInt_Limit /* = NULL */)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	unordered_map<string, SESSION_USERINFO>::const_iterator stl_MapIterator = stl_MapUser.find(lpszUser);
	if (stl_MapIterator == stl_MapUser.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	
	if (_tcsxlen(lpszPass) != _tcsxlen(stl_MapIterator->second.tszUserPass))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PASSWORD;
		st_Locker.unlock_shared();
		return false;
	}
	if (0 != _tcsxncmp(lpszPass, stl_MapIterator->second.tszUserPass, _tcsxlen(lpszPass)))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_PASSWORD;
		st_Locker.unlock_shared();
		return false;
	}
	if (NULL != pInt_Limit)
	{
		*pInt_Limit = _ttxoi(stl_MapIterator->second.tszUserLimit);
	}
	st_Locker.unlock_shared();
	return true;
}