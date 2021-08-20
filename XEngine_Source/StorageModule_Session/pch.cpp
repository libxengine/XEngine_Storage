#include "pch.h"
#include "Session_Stroage/Session_DLStroage.h"
#include "Session_Stroage/Session_UPStroage.h"
#include "Session_User/Session_User.h"
/********************************************************************
//    Created:     2021/06/02  14:38:34
//    File Name:   D:\XEngine_Storage\StorageModule_Session\pch.cpp
//    File Path:   D:\XEngine_Storage\StorageModule_Session
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话导出实现
//    History:
*********************************************************************/
BOOL Session_IsErrorOccur = FALSE;
DWORD Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CSession_DLStroage m_DLStorage;
CSession_UPStroage m_UPStorage;
CSession_User m_User;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义机器
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD Session_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/************************************************************************/
/*                        用户管理导出的函数                            */
/************************************************************************/
extern "C" BOOL Session_User_Init(LPCTSTR lpszUserFile)
{
	return m_User.Session_User_Init(lpszUserFile);
}
extern "C" BOOL Session_User_Destory()
{
	return m_User.Session_User_Destory();
}
extern "C" BOOL Session_User_Exist(LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	return m_User.Session_User_Exist(lpszUser, lpszPass);
}
/************************************************************************/
/*                        存储会话导出的函数                            */
/************************************************************************/
extern "C" BOOL Session_DLStroage_Init(int nPoolCount, int nTryTime, int nAutoSpeed)
{
	return m_DLStorage.Session_DLStroage_Init(nPoolCount, nTryTime, nAutoSpeed);
}
extern "C" BOOL Session_DLStroage_Destory()
{
	return m_DLStorage.Session_DLStroage_Destory();
}
extern "C" BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x * pInt_Count, __int64x * pInt_LeftCount, int nPosStart, int nPostEnd)
{
	return m_DLStorage.Session_DLStroage_Insert(lpszClientAddr, lpszFileDir, pInt_Count, pInt_LeftCount, nPosStart, nPostEnd);
}
extern "C" BOOL Session_DLStroage_GetList(int nPool, LPCTSTR lpszClientAddr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_DLStorage.Session_DLStroage_GetList(nPool, lpszClientAddr, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Session_DLStroage_GetInfo(int nPool, LPCTSTR lpszClientAddr, SESSION_STORAGEINFO * pSt_StorageInfo)
{
	return m_DLStorage.Session_DLStroage_GetInfo(nPool, lpszClientAddr, pSt_StorageInfo);
}
extern "C" BOOL Session_DLStroage_GetCount(int nIndex, list<string>*pStl_ListClient)
{
	return m_DLStorage.Session_DLStroage_GetCount(nIndex, pStl_ListClient);
}
extern "C" BOOL Session_DLStorage_SetSeek(LPCTSTR lpszClientAddr, int nSeek, BOOL bError, SESSION_STORAGEDYNAMICRATE * pSt_StorageRate)
{
	return m_DLStorage.Session_DLStorage_SetSeek(lpszClientAddr, nSeek, bError, pSt_StorageRate);
}
extern "C" BOOL Session_DLStroage_Delete(LPCTSTR lpszClientAddr)
{
	return m_DLStorage.Session_DLStroage_Delete(lpszClientAddr);
}
extern "C" BOOL Session_UPStroage_Init()
{
	return m_UPStorage.Session_UPStroage_Init();
}
extern "C" BOOL Session_UPStroage_Destory()
{
	return m_UPStorage.Session_UPStroage_Destory();
}
extern "C" BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x nFileSize, __int64x nLeftCount, int nPosStart, int nPostEnd)
{
	return m_UPStorage.Session_UPStroage_Insert(lpszClientAddr, lpszFileDir, nFileSize, nLeftCount, nPosStart, nPostEnd);
}
extern "C" BOOL Session_UPStroage_GetComplete(LPCTSTR lpszClientAddr, BOOL * pbComplete)
{
	return m_UPStorage.Session_UPStroage_GetComplete(lpszClientAddr, pbComplete);
}
extern "C" BOOL Session_UPStroage_GetInfo(LPCTSTR lpszClientAddr, SESSION_STORAGEINFO * pSt_StorageInfo)
{
	return m_UPStorage.Session_UPStroage_GetInfo(lpszClientAddr, pSt_StorageInfo);
}
extern "C" BOOL Session_UPStroage_Write(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_UPStorage.Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
extern "C" BOOL Session_UPStroage_Exist(LPCTSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_Exist(lpszClientAddr);
}
extern "C" BOOL Session_UPStroage_Delete(LPCTSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_Delete(lpszClientAddr);
}