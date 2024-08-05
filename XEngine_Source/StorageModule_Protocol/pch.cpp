#include "pch.h"
#include "Protocol_Parse/Protocol_StorageParse.h"
#include "Protocol_Packet/Protocol_StoragePacket.h"
/********************************************************************
//    Created:     2021/10/25  14:38:54
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Protocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
bool Protocol_IsErrorOccur = false;
XLONG Protocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CProtocol_StoragePacket m_StoragePacket;
CProtocol_StorageParse m_StorageParse;
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Protocol_GetLastError(int* pInt_ErrorCode)
{
	if (NULL != pInt_ErrorCode)
	{
		*pInt_ErrorCode = errno;
	}
	return Protocol_dwErrorCode;
}
/************************************************************************/
/*                        解析协议导出                                  */
/************************************************************************/
extern "C" bool Protocol_StorageParse_ProxyNotify(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszClientAddr, XCHAR * ptszBuckKey, XCHAR * ptszFileName, XCHAR * ptszFileHash, __int64x * pInt_FileSize)
{
	return m_StorageParse.Protocol_StorageParse_ProxyNotify(lpszMsgBuffer, nMsgLen, ptszClientAddr, ptszBuckKey, ptszFileName, ptszFileHash, pInt_FileSize);
}
extern "C" bool Protocol_StorageParse_QueryFile(LPCXSTR lpszMsgBuffer, XCHAR * ptszTimeStart, XCHAR * ptszTimeEnd, XCHAR * ptszBuckKey, XCHAR * ptszFileName, XCHAR * ptszFileHash, int* pInt_Mode, XNETHANDLE * pxhToken)
{
	return m_StorageParse.Protocol_StorageParse_QueryFile(lpszMsgBuffer, ptszTimeStart, ptszTimeEnd, ptszBuckKey, ptszFileName, ptszFileHash, pInt_Mode, pxhToken);
}
extern "C" bool Protocol_StorageParse_ReportFile(LPCXSTR lpszMsgBuffer, int nMsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int* pInt_ListCount)
{
	return m_StorageParse.Protocol_StorageParse_ReportFile(lpszMsgBuffer, nMsgLen, pppSt_DBFile, pInt_ListCount);
}
extern "C" bool Protocol_StorageParse_DirOperator(LPCXSTR lpszMsgBuffer, XCHAR * ptszUserDir, XCHAR * ptszBuckKey, int* pInt_Operator)
{
	return m_StorageParse.Protocol_StorageParse_DirOperator(lpszMsgBuffer, ptszUserDir, ptszBuckKey, pInt_Operator);
}
extern "C" bool Protocol_StorageParse_SpeedLimit(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Code, int* pInt_Limit)
{
	return m_StorageParse.Protocol_StorageParse_SpeedLimit(lpszMsgBuffer, nMsgLen, pInt_Code, pInt_Limit);
}
extern "C" bool Protocol_StorageParse_P2PToken(LPCXSTR lpszMsgBuffer, int nMsgLen, XNETHANDLE * pxhToken)
{
	return m_StorageParse.Protocol_StorageParse_P2PToken(lpszMsgBuffer, nMsgLen, pxhToken);
}
extern "C" bool Protocol_StorageParse_Action(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_ACTIONINFO * pSt_ActionInfo)
{
	return m_StorageParse.Protocol_StorageParse_Action(lpszMsgBuffer, nMsgLen, pSt_ActionInfo);
}
extern "C" bool Protocol_StorageParse_WDLock(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock)
{
	return m_StorageParse.Protocol_StorageParse_WDLock(lpszMsgBuffer, nMsgLen, pSt_WDLock);
}
/************************************************************************/
/*                        打包协议导出                                  */
/************************************************************************/
extern "C" bool Protocol_StoragePacket_BasicAuth(LPCXSTR lpszMethod, LPCXSTR lpszPostUrl, LPCXSTR lpszClientAddr, LPCXSTR lpszUser, LPCXSTR lpszPass, XCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_StoragePacket.Protocol_StoragePacket_BasicAuth(lpszMethod, lpszPostUrl, lpszClientAddr, lpszUser, lpszPass, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" bool Protocol_StoragePacket_UPDown(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszBuckKey, LPCXSTR lpszFileName, LPCXSTR lpszClientAddr, __int64x nFileSize, bool bDown, LPCXSTR lpszFileHash)
{
	return m_StoragePacket.Protocol_StoragePacket_UPDown(ptszMsgBuffer, pInt_MsgLen, lpszBuckKey, lpszFileName, lpszClientAddr, nFileSize, bDown, lpszFileHash);
}
extern "C" bool Protocol_StoragePacket_QueryFile(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int nListCount, LPCXSTR lpszTimeStart, LPCXSTR lpszTimeEnd, XNETHANDLE xhToken)
{
	return m_StoragePacket.Protocol_StoragePacket_QueryFile(ptszMsgBuffer, pInt_MsgLen, pppSt_DBFile, nListCount, lpszTimeStart, lpszTimeEnd, xhToken);
}
extern "C" bool Protocol_StoragePacket_Info(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, SESSION_STORAGEINFO * **pppSt_DLInfo, SESSION_STORAGEINFO * **pppSt_UPInfo, int nDLCount, int nUPCount)
{
	return m_StoragePacket.Protocol_StoragePacket_Info(ptszMsgBuffer, pInt_MsgLen, pppSt_DLInfo, pppSt_UPInfo, nDLCount, nUPCount);
}
extern "C" bool Protocol_StoragePacket_DirOperator(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XCHAR * **pppszListEnum, int nListCount)
{
	return m_StoragePacket.Protocol_StoragePacket_DirOperator(ptszMsgBuffer, pInt_MsgLen, pppszListEnum, nListCount);
}
extern "C" bool Protocol_StoragePacket_REQFile(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszFileName, LPCXSTR lpszFileHash, XNETHANDLE xhToken)
{
	return m_StoragePacket.Protocol_StoragePacket_REQFile(ptszMsgBuffer, pInt_MsgLen, lpszFileName, lpszFileHash, xhToken);
}
extern "C" bool Protocol_StoragePacket_Action(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, XENGINE_ACTIONINFO * pSt_ActionInfo)
{
	return m_StoragePacket.Protocol_StoragePacket_Action(ptszMsgBuffer, pInt_MsgLen, xhToken, pSt_ActionInfo);
}
extern "C" bool Protocol_StoragePacket_WDPropfind(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XCHAR*** ppptszListFile, int nFileCount, LPCXSTR lpszBucketPath, LPCXSTR lpszBucketKey)
{
	return m_StoragePacket.Protocol_StoragePacket_WDPropfind(ptszMsgBuffer, pInt_MsgLen, ppptszListFile, nFileCount, lpszBucketPath, lpszBucketKey);
}
extern "C" bool Protocol_StoragePacket_WDLock(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_WEBDAVLOCK* pSt_WDLock)
{
	return m_StoragePacket.Protocol_StoragePacket_WDLock(ptszMsgBuffer, pInt_MsgLen, pSt_WDLock);
}