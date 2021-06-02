#include "pch.h"
#include "Session_Client.h"
/********************************************************************
//    Created:     2021/05/08  13:55:45
//    File Name:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session\Session_Client\Session_Client.cpp
//    File Path:   D:\XEngine_APPService\XEngine_Source\XEngine_NetHelp\NetHelp_Session\Session_Client
//    File Base:   Session_Client
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端会话类
//    History:
*********************************************************************/
CSession_Client::CSession_Client()
{
}
CSession_Client::~CSession_Client()
{
}
//////////////////////////////////////////////////////////////////////////
//                      公有函数
//////////////////////////////////////////////////////////////////////////
BOOL CSession_Client::Session_Client_Insert(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == pSt_ProtocolUser))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_INSERT_PARAMENT;
		return FALSE;
	}

	st_Locker.lock();
	unordered_map<tstring, SESSION_CLIENT>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_INSERT_EXIST;
		st_Locker.unlock();
		return FALSE;
	}
	SESSION_CLIENT st_Client;
	memset(&st_Client, '\0', sizeof(SESSION_CLIENT));

	memcpy(&st_Client.st_UserInfo, pSt_ProtocolUser, sizeof(XENGINE_PROTOCOL_USERREG));

	stl_MapClient.insert(make_pair(lpszClientAddr, st_Client));
	st_Locker.unlock();
	return TRUE;
}
BOOL CSession_Client::Session_Client_Get(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == pSt_ProtocolUser))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_GET_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_CLIENT>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_GET_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	memcpy(pSt_ProtocolUser, &stl_MapIterator->second.st_UserInfo, sizeof(XENGINE_PROTOCOL_USERREG));
	st_Locker.unlock_shared();
	return TRUE;
}
BOOL CSession_Client::Session_Client_Delete(LPCTSTR lpszClientAddr, XENGINE_PROTOCOL_USERREG* pSt_ProtocolUser /* = NULL */)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszClientAddr) || (NULL == pSt_ProtocolUser))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_DELETE_PARAMENT;
		return FALSE;
	}

	st_Locker.lock_shared();
	unordered_map<tstring, SESSION_CLIENT>::const_iterator stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_XENGINE_NETHELP_SESSION_CLIENT_DELETE_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	if (NULL != pSt_ProtocolUser)
	{
		memcpy(pSt_ProtocolUser, &stl_MapIterator->second.st_UserInfo, sizeof(XENGINE_PROTOCOL_USERREG));
	}
	st_Locker.unlock_shared();
	return TRUE;
}