#include "pch.h"
#include "XStorageProtocol_Comm/XStorageProtocol_Comm.h"
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
CXStorageProtocol_Comm m_ProtocolComm;
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
/*                      公用协议操作导出函数                            */
/************************************************************************/
extern "C" BOOL XStorageProtocol_Comm_ParseFile(LPCTSTR lpszMsgBuffer, int nMsgLen, BYTE byVersion, XENGINE_PROTOCOLFILE * pSt_ProtocolFile)
{
	return m_ProtocolComm.XStorageProtocol_Comm_ParseFile(lpszMsgBuffer, nMsgLen, byVersion, pSt_ProtocolFile);
}
extern "C" BOOL XStorageProtocol_Comm_Response(XENGINE_PROTOCOLHDR * pSt_ProtocolHdr, TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nMsgCode, LPCTSTR lpszMsgInfo)
{
	return m_ProtocolComm.XStorageProtocol_Comm_Response(pSt_ProtocolHdr, ptszMsgBuffer, pInt_MsgLen, nMsgCode, lpszMsgInfo);
}
extern "C" BOOL XStorageProtocol_Comm_ParseClient(LPCTSTR lpszMsgBuffer, TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr)
{
	return m_ProtocolComm.XStorageProtocol_Comm_ParseClient(lpszMsgBuffer, ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolHdr);
}
/************************************************************************/
/*                      客户端协议操作导出函数                          */
/************************************************************************/
extern "C" BOOL XStorageProtocol_Client_REQQueryFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFileName /* = NULL */, LPCTSTR lpszFileMD5 /* = NULL */)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQQueryFile(ptszMsgBuffer, pInt_MsgLen, lpszTimeStart, lpszTimeEnd, lpszFileName, lpszFileMD5);
}
extern "C" BOOL XStorageProtocol_Client_REQQueryUser(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserName, BOOL bQueryKey)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQQueryUser(ptszMsgBuffer, pInt_MsgLen, lpszUserName, bQueryKey);
}
extern "C" BOOL XStorageProtocol_Client_REQDelete(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFileName, LPCTSTR lpszFileMD5)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQDelete(ptszMsgBuffer, pInt_MsgLen, lpszFileName, lpszFileMD5);
}
extern "C" BOOL XStorageProtocol_Client_REQLogin(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOL_USERAUTH * pSt_ProtocolAuth)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQLogin(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolAuth);
}
extern "C" BOOL XStorageProtocol_Client_REQFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, LPCTSTR lpszFileMD5, LPCTSTR lpszFileName, LPCTSTR lpszFilePath, BOOL bUPFile)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQFile(ptszMsgBuffer, pInt_MsgLen, xhToken, lpszFileMD5, lpszFileName, lpszFilePath, bUPFile);
}
//////////////////////////////////////////////////////////////////////////
extern "C" BOOL XStorageProtocol_Client_REQDirOperator(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUserDir, BOOL bCreate)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQDirOperator(ptszMsgBuffer, pInt_MsgLen, lpszUserDir, bCreate);
}
extern "C" BOOL XStorageProtocol_Client_REQDirQuery(TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQDirQuery(ptszMsgBuffer, pInt_MsgLen);
}
extern "C" BOOL XStorageProtocol_Client_REQRegister(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszUser, LPCTSTR lpszPass, LPCTSTR lpszEMailAddr, __int64x nPhoneNumber, __int64x nIDNumber, int nPerimission)
{
	return m_ProtocolClient.XStorageProtocol_Client_REQRegister(ptszMsgBuffer, pInt_MsgLen, lpszUser, lpszPass, lpszEMailAddr, nPhoneNumber, nIDNumber, nPerimission);
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
extern "C" BOOL XStorageProtocol_Core_REPFile(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XNETHANDLE xhToken, BOOL bUPFile, int nCode, LPCTSTR lpszCodeMsg, XENGINE_PROTOCOLFILE * pSt_ProtcolFile)
{
	return m_ProtocolCore.XStorageProtocol_Core_REPFile(ptszMsgBuffer, pInt_MsgLen, xhToken, bUPFile, nCode, lpszCodeMsg, pSt_ProtcolFile);
}
extern "C" BOOL XStorageProtocol_Core_REQCreateDir(LPCTSTR lpszMsgBuffer, TCHAR * ptszUserDir)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQCreateDir(lpszMsgBuffer, ptszUserDir);
}
extern "C" BOOL XStorageProtocol_Core_REPQueryDir(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, TCHAR * **pppszListEnum, int nListCount)
{
	return m_ProtocolCore.XStorageProtocol_Core_REPQueryDir(ptszMsgBuffer, pInt_MsgLen, pppszListEnum, nListCount);
}
extern "C" BOOL XStorageProtocol_Core_REQUserReg(LPCTSTR lpszMsgBuffer, XSTORAGECORE_USERINFO * pSt_ProtocolRegister)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQUserReg(lpszMsgBuffer, pSt_ProtocolRegister);
}
extern "C" BOOL XStorageProtocol_Core_REQQueryUser(LPCTSTR lpszMsgBuffer, TCHAR * ptszUserName, BOOL * pbKeyQuery)
{
	return m_ProtocolCore.XStorageProtocol_Core_REQQueryUser(lpszMsgBuffer, ptszUserName, pbKeyQuery);
}
extern "C" BOOL XStorageProtocol_Core_REPQueryUser(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPVOID lParam, BOOL bKeyInfo)
{
	return m_ProtocolCore.XStorageProtocol_Core_REPQueryUser(ptszMsgBuffer, pInt_MsgLen, lParam, bKeyInfo);
}
