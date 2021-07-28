#include "pch.h"
#include "XStorageProtocol_Proxy/XStorageProtocol_Proxy.h"
#include "XStorageProtocol_Core/XStorageProtocol_Core.h"
#include "XStorageProtocol_Client/XStorageProtocol_Client.h"
/********************************************************************
//	Created:	2019/6/28   16:02
//	Filename: 	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol\NetEngine_XStorageProtocol.cpp
//	File Path:	E:\NetEngine_Windows\NetEngine_SourceCode\NetEngine_Storage\NetEngine_XStorageProtocol
//	File Base:	NetEngine_XStorageProtocol
//	File Ext:	cpp
//  Project:    NetEngine(网络通信引擎)
//	Author:		qyt
//	Purpose:	导出函数实现
//	History:
*********************************************************************/
BOOL XStorage_IsErrorOccur = FALSE;
DWORD XStorage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CXStorageProtocol_Proxy m_ProtocolProxy;
CXStorageProtocol_Core m_ProtocolCore;
CXStorageProtocol_Client m_ProtocolClient;
//////////////////////////////////////////////////////////////////////////
//                      导出函数实现
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XStorageProtocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return XStorage_dwErrorCode;
}
/************************************************************************/
/*                      代理协议操作导出函数                            */
/************************************************************************/
extern "C" BOOL XStorageProtocol_Proxy_PacketBasicAuth(LPCTSTR lpszMethod, LPCTSTR lpszPostUrl, LPCTSTR lpszClientAddr, LPCTSTR lpszUser, LPCTSTR lpszPass, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_ProtocolProxy.XStorageProtocol_Proxy_PacketBasicAuth(lpszMethod, lpszPostUrl, lpszClientAddr, lpszUser, lpszPass, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL XStorageProtocol_Proxy_PacketUPDown(LPCTSTR lpszFileName, LPCTSTR lpszClientAddr, __int64x nFileSize, TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileHash)
{
	return m_ProtocolProxy.XStorageProtocol_Proxy_PacketUPDown(lpszFileName, lpszClientAddr, nFileSize, ptszMsgBuffer, pInt_MsgLen, lpszFileHash);
}
extern "C" BOOL XStorageProtocol_Proxy_ParseNotify(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszClientAddr, TCHAR * ptszFileName, TCHAR * ptszFileHash, __int64x * pInt_FileSize)
{
	return m_ProtocolProxy.XStorageProtocol_Proxy_ParseNotify(lpszMsgBuffer, nMsgLen, ptszClientAddr, ptszFileName, ptszFileHash, pInt_FileSize);
}
/************************************************************************/
/*                      客户端协议操作导出函数                          */
/************************************************************************/
extern "C" BOOL XStorageProtocol_Client_REQQueryFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileHash /* = NULL */)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQQueryFile(ptszMsgBuffer, pInt_MsgLen, lpszFileName, lpszFileHash);
}
extern "C" BOOL XStorageProtocol_Client_REQDelete(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileName, LPCTSTR lpszFileHash)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQDelete(ptszMsgBuffer, pInt_MsgLen, lpszFileName, lpszFileHash);
}
extern "C" BOOL XStorageProtocol_Client_REQDirOperator(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserDir, int nOperator)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQDirOperator(ptszMsgBuffer, pInt_MsgLen, lpszUserDir, nOperator);
}
/************************************************************************/
/*                      服务核心协议操作导出函数                        */
/************************************************************************/
extern "C" BOOL XStorageProtocol_Core_REQQueryFile(LPCTSTR lpszMsgBuffer, TCHAR * ptszTimeStart, TCHAR * ptszTimeEnd, TCHAR * ptszFileName, TCHAR * ptszFileHash)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQQueryFile(lpszMsgBuffer, ptszTimeStart, ptszTimeEnd, ptszFileName, ptszFileHash);
}
extern "C" BOOL XStorageProtocol_Core_REPQueryFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XSTORAGECORE_DBFILE * **pppSt_DBFile, int nListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
	return m_ProtocolCore.XStorageProtocol_Core_REPQueryFile(ptszMsgBuffer, pInt_MsgLen, pppSt_DBFile, nListCount, lpszTimeStart, lpszTimeEnd);
}
extern "C" BOOL XStorageProtocol_Core_REQDirOperator(LPCTSTR lpszMsgBuffer, TCHAR * ptszUserDir, int* pInt_Operator)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQDirOperator(lpszMsgBuffer, ptszUserDir, pInt_Operator);
}
extern "C" BOOL XStorageProtocol_Core_REPDirOperator(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, TCHAR * **pppszListEnum, int nListCount)
{
	return m_ProtocolCore.XStorageProtocol_Core_REPDirOperator(ptszMsgBuffer, pInt_MsgLen, pppszListEnum, nListCount);
}
extern "C" BOOL XStorageProtocol_Core_REQUPEvent(LPCTSTR lpszMsgBuffer, LPCTSTR lpszBoundary, TCHAR * ptszFileName, TCHAR * ptszFilePath, TCHAR * ptszFileHash, __int64x * pInt_FileSize)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQUPEvent(lpszMsgBuffer, lpszBoundary, ptszFileName, ptszFilePath, ptszFileHash, pInt_FileSize);
}