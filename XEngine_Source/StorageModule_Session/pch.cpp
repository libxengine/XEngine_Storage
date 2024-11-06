#include "pch.h"
#include "Session_Stroage/Session_DLStroage.h"
#include "Session_Stroage/Session_UPStroage.h"
#include "Session_Action/Session_Action.h"
#include "Session_Webdav/Session_Webdav.h"
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
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CSession_DLStroage m_DLStorage;
CSession_UPStroage m_UPStorage;
CSession_Action m_Action;
CSession_Webdav m_Webdav;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义机器
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Session_GetLastError(int* pInt_SysError)
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
extern "C" bool Session_DLStroage_Init(int nMaxConnect)
{
	return m_DLStorage.Session_DLStroage_Init(nMaxConnect);
}
extern "C" bool Session_DLStroage_Destory()
{
	return m_DLStorage.Session_DLStroage_Destory();
}
extern "C" bool Session_DLStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, __int64x * pInt_Count, __int64x * pInt_LeftCount, int nPosStart, int nPostEnd, LPCXSTR lpszFileHash, int nLimit, XHANDLE xhToken)
{
	return m_DLStorage.Session_DLStroage_Insert(lpszClientAddr, lpszBuckKey, lpszFileDir, pInt_Count, pInt_LeftCount, nPosStart, nPostEnd, lpszFileHash, nLimit, xhToken);
}
extern "C" bool Session_DLStroage_GetBuffer(LPCXSTR lpszClientAddr, XCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_DLStorage.Session_DLStroage_GetBuffer(lpszClientAddr, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" bool Session_DLStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO * pSt_StorageInfo)
{
	return m_DLStorage.Session_DLStroage_GetInfo(lpszClientAddr, pSt_StorageInfo);
}
extern "C" bool Session_DLStroage_GetCount(int* pInt_ListCount)
{
	return m_DLStorage.Session_DLStroage_GetCount(pInt_ListCount);
}
extern "C" bool Session_DLStorage_SetSeek(LPCXSTR lpszClientAddr, int nSeek)
{
	return m_DLStorage.Session_DLStorage_SetSeek(lpszClientAddr, nSeek);
}
extern "C" bool Session_DLStorage_GetAll(SESSION_STORAGEINFO * **pppSt_StorageInfo, int* pInt_ListCount)
{
	return m_DLStorage.Session_DLStorage_GetAll(pppSt_StorageInfo, pInt_ListCount);
}
extern "C" bool Session_DLStroage_Delete(LPCXSTR lpszClientAddr)
{
	return m_DLStorage.Session_DLStroage_Delete(lpszClientAddr);
}
extern "C" bool Session_DLStroage_MaxConnect(LPCXSTR lpszClientAddr)
{
	return m_DLStorage.Session_DLStroage_MaxConnect(lpszClientAddr);
}
extern "C" bool Session_UPStroage_Init(int nMaxConnect, bool bUPResume)
{
	return m_UPStorage.Session_UPStroage_Init(nMaxConnect, bUPResume);
}
extern "C" bool Session_UPStroage_Destory()
{
	return m_UPStorage.Session_UPStroage_Destory();
}
extern "C" bool Session_UPStroage_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszBuckKey, LPCXSTR lpszFileDir, XHANDLE xhSpeed, __int64x nFileSize, bool bRewrite, int nSpeedLimit, int nPosStart, int nPostEnd)
{
	return m_UPStorage.Session_UPStroage_Insert(lpszClientAddr, lpszBuckKey, lpszFileDir, xhSpeed, nFileSize, bRewrite, nSpeedLimit, nPosStart, nPostEnd);
}
extern "C" bool Session_UPStroage_GetInfo(LPCXSTR lpszClientAddr, SESSION_STORAGEINFO * pSt_StorageInfo)
{
	return m_UPStorage.Session_UPStroage_GetInfo(lpszClientAddr, pSt_StorageInfo);
}
extern "C" XHANDLE Session_UPStroage_GetSpeed(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_GetSpeed(lpszClientAddr);
}
extern "C" bool Session_UPStroage_Write(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	return m_UPStorage.Session_UPStroage_Write(lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
extern "C" bool Session_UPStroage_Exist(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_Exist(lpszClientAddr);
}
extern "C" bool Session_UPStorage_GetAll(SESSION_STORAGEINFO * **pppSt_StorageInfo, int* pInt_ListCount)
{
	return m_UPStorage.Session_UPStorage_GetAll(pppSt_StorageInfo, pInt_ListCount);
}
extern "C" bool Session_UPStroage_Delete(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_Delete(lpszClientAddr);
}
extern "C" bool Session_UPStroage_Close(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_Close(lpszClientAddr);
}
extern "C" bool Session_UPStroage_MaxConnect(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_MaxConnect(lpszClientAddr);
}
extern "C" bool Session_UPStroage_SetBoundary(LPCXSTR lpszClientAddr, LPCXSTR lpszBoundary)
{
	return m_UPStorage.Session_UPStroage_SetBoundary(lpszClientAddr, lpszBoundary);
}
extern "C" bool Session_UPStroage_SetBoundaryStart(LPCXSTR lpszClientAddr)
{
	return m_UPStorage.Session_UPStroage_SetBoundaryStart(lpszClientAddr);
}
/************************************************************************/
/*                        存储会话导出的函数                            */
/************************************************************************/
extern "C" bool Session_Action_Insert(XNETHANDLE xhToken, XHANDLE xhAction, XENGINE_ACTIONINFO * pSt_ActionInfo)
{
	return m_Action.Session_Action_Insert(xhToken, xhAction, pSt_ActionInfo);
}
extern "C" XHANDLE Session_Action_GetToken(XNETHANDLE xhToken)
{
	return m_Action.Session_Action_GetToken(xhToken);
}
extern "C" bool Session_Action_GetInfo(XNETHANDLE xhToken, XENGINE_ACTIONINFO * pSt_ActionInfo)
{
	return m_Action.Session_Action_GetInfo(xhToken, pSt_ActionInfo);
}
extern "C" bool Session_Action_GetAll(XNETHANDLE * **pppxhToken, int* pInt_ListCount)
{
	return m_Action.Session_Action_GetAll(pppxhToken, pInt_ListCount);
}
extern "C" bool Session_Action_Delete(XNETHANDLE xhToken)
{
	return m_Action.Session_Action_Delete(xhToken);
}
/************************************************************************/
/*                        WEBDAV会话导出的函数                          */
/************************************************************************/
extern "C" bool Session_Webdav_Insert(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker, int nTimeout)
{
	return m_Webdav.Session_Webdav_Insert(lpszFileName, pSt_WDLocker, nTimeout);
}
extern "C" bool Session_Webdav_Get(LPCXSTR lpszFileName, XENGINE_WEBDAVLOCK* pSt_WDLocker)
{
	return m_Webdav.Session_Webdav_Get(lpszFileName, pSt_WDLocker);
}
extern "C" bool Session_Webdav_Delete(LPCXSTR lpszFileName)
{
	return m_Webdav.Session_Webdav_Delete(lpszFileName);
}