#include "pch.h"
#include "Session_FTPContral.h"
/********************************************************************
//    Created:     2024/08/05  16:14:56
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_FTPContral\Session_FTPContral.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Session\Session_FTPContral
//    File Base:   Session_FTPContral
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     FTP会话
//    History:
*********************************************************************/
CSession_FTPContral::CSession_FTPContral()
{
}
CSession_FTPContral::~CSession_FTPContral()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：Session_FTP_Insert
函数功能：插入一个会话
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：bUPLoader
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是否为上传会话
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_Insert(LPCXSTR lpszClientAddr, bool bUPLoader)
{
	Session_IsErrorOccur = false;

	SESSION_FTPCONTRAL st_FTPSession = {};

	st_FTPSession.nTimeStart = time(NULL);
	st_FTPSession.bUPLoader = bUPLoader;

	st_Locker.lock();
	stl_MapAction.insert(make_pair(lpszClientAddr, st_FTPSession));
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：Session_FTP_Set
函数功能：设置
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要操作的文件
 参数.三：lpszFilePath
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要操作的文件路径
 参数.四：lpszAlisName
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入路径别名
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_Set(LPCXSTR lpszClientAddr, LPCXSTR lpszFileName, LPCXSTR lpszFilePath, LPCXSTR lpszAlisName)
{
	Session_IsErrorOccur = false;

	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);

	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return NULL;
	}

	if (NULL != lpszFileName)
	{
		memset(stl_MapIterator->second.tszFileName, '\0', sizeof(stl_MapIterator->second.tszFileName));
		_tcsxcpy(stl_MapIterator->second.tszFileName, lpszFileName);
	}
	if (NULL != lpszFilePath)
	{
		memset(stl_MapIterator->second.tszFilePath, '\0', sizeof(stl_MapIterator->second.tszFilePath));
		_tcsxcpy(stl_MapIterator->second.tszFilePath, lpszFilePath);
	}
	if (NULL != lpszAlisName)
	{
		memset(stl_MapIterator->second.tszAlisPath, '\0', sizeof(stl_MapIterator->second.tszAlisPath));
		_tcsxcpy(stl_MapIterator->second.tszAlisPath, lpszAlisName);
	}
	return true;
}
/********************************************************************
函数名称：Session_FTP_Get
函数功能：获取信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
 参数.二：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出操作的文件
 参数.三：ptszFilePath
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出操作的文件路径
 参数.四：ptszAlisPath
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出操作的路径别名
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_Get(LPCXSTR lpszClientAddr, XCHAR* ptszFileName, XCHAR* ptszFilePath, XCHAR* ptszAlisPath)
{
	Session_IsErrorOccur = false;

	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);
	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return NULL;
	}

	if (NULL != ptszFilePath)
	{
		_tcsxcpy(ptszFilePath, stl_MapIterator->second.tszFilePath);
	}
	if (NULL != ptszFileName)
	{
		_tcsxcpy(ptszFileName, stl_MapIterator->second.tszFileName);
	}
	if (NULL != ptszAlisPath)
	{
		_tcsxcpy(ptszAlisPath, stl_MapIterator->second.tszAlisPath);
	}
	return true;
}
/********************************************************************
函数名称：Session_FTP_SetSocket
函数功能：设置网络句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入操作的端口
 参数.二：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入操作的套接字句柄
返回值
  类型：逻辑型
  意思：是否设置成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_SetSocket(LPCXSTR lpszClientAddr, int nPort, XHANDLE xhToken)
{
	Session_IsErrorOccur = false;
	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);
	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return NULL;
	}
	stl_MapIterator->second.nPort = nPort;
	stl_MapIterator->second.xhToken = xhToken;
	return true;
}
/********************************************************************
函数名称：Session_FTP_GetSocket
函数功能：获取网络句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
 参数.二：pInt_Port
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出操作的端口
返回值
  类型：句柄
  意思：返回获取到的网络句柄
备注：
*********************************************************************/
XHANDLE CSession_FTPContral::Session_FTP_GetSocket(LPCXSTR lpszClientAddr, int* pInt_Port)
{
	Session_IsErrorOccur = false;

	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);
	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return NULL;
	}

	if (NULL != pInt_Port)
	{
		*pInt_Port = stl_MapIterator->second.nPort;
	}
	return stl_MapIterator->second.xhToken;
}
/********************************************************************
函数名称：Session_FTP_SetRetr
函数功能：设置断点续传
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
 参数.二：nPos
  In/Out：In
  类型：整数型
  可空：N
  意思：输入断点续传位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_SetRetr(LPCXSTR lpszClientAddr, __int64u nPos)
{
	Session_IsErrorOccur = false;

	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);
	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return false;
	}
	stl_MapIterator->second.nPos = nPos;
	return true;
}
/********************************************************************
函数名称：Session_FTP_GetRetr
函数功能：获取断点续传
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
 参数.二：pInt_Pos
  In/Out：Out
  类型：整数型指针
  可空：Y
  意思：输出断点续传位置
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_GetRetr(LPCXSTR lpszClientAddr, __int64u* pInt_Pos)
{
	Session_IsErrorOccur = false;

	std::shared_lock<std::shared_mutex> st_AutoLock(st_Locker);
	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapAction.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STORAGE_MODULE_SESSION_NOTFOUND;
		return false;
	}
	*pInt_Pos = stl_MapIterator->second.nPort;
	return true;
}
/********************************************************************
函数名称：Session_FTP_Delete
函数功能：删除一个会话
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CSession_FTPContral::Session_FTP_Delete(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	std::unique_lock<std::shared_mutex> st_AutoLock(st_Locker);

	unordered_map<xstring, SESSION_FTPCONTRAL>::iterator stl_MapIterator = stl_MapAction.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapAction.end())
	{
		stl_MapAction.erase(stl_MapIterator);
	}
	return true;
}