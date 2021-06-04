#include "pch.h"
#include "Session_Stroage/Session_DLStroage.h"
#include "Session_Stroage/Session_UPStroage.h"
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
/*                        存储会话导出的函数                            */
/************************************************************************/
extern "C" BOOL Session_DLStroage_Init(int nPoolCount)
{
	return m_DLStorage.Session_DLStroage_Init(nPoolCount);
}
extern "C" BOOL Session_DLStroage_Destory()
{
	return m_DLStorage.Session_DLStroage_Destory();
}
extern "C" BOOL Session_DLStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x * pInt_Count, int nPos)
{
	return m_DLStorage.Session_DLStroage_Insert(lpszClientAddr, lpszFileDir, pInt_Count, nPos);
}
extern "C" BOOL Session_DLStroage_GetList(int nPool, int nIndex, TCHAR * ptszClientAddr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_DLStorage.Session_DLStroage_GetList(nPool, nIndex, ptszClientAddr, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL Session_DLStroage_GetCount(int nIndex, int* pInt_ListCount)
{
	return m_DLStorage.Session_DLStroage_GetCount(nIndex, pInt_ListCount);
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
extern "C" BOOL Session_UPStroage_Insert(LPCTSTR lpszClientAddr, LPCTSTR lpszFileDir, __int64x nFileSize, int nPos)
{
	return m_UPStorage.Session_UPStroage_Insert(lpszClientAddr, lpszFileDir, nFileSize, nPos);
}
extern "C" BOOL Session_UPStroage_GetComplete(LPCTSTR lpszClientAddr, BOOL * pbComplete)
{
	return m_UPStorage.Session_UPStroage_GetComplete(lpszClientAddr, pbComplete);
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