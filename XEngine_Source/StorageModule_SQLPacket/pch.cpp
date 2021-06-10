#include "pch.h"
#include "SQLPacket_File/SQLPacket_File.h"
/********************************************************************
//    Created:     2021/06/09  13:57:16
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL SQLPacket_IsErrorOccur = FALSE;
DWORD SQLPacket_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CSQLPacket_File m_SQLFile;
//////////////////////////////////////////////////////////////////////////
//                        导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD SQLPacket_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return SQLPacket_dwErrorCode;
}
/************************************************************************/
/*                        公用                                          */
/************************************************************************/
extern "C" BOOL SQLPacket_Connect(LPCTSTR lpszSQLAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	if (!m_SQLFile.SQLPacket_File_Connect(lpszSQLAddr, nPort, lpszUser, lpszPass))
	{
		return FALSE;
	}
	return TRUE;
}
extern "C" BOOL SQLPacket_Close()
{
	m_SQLFile.SQLPacket_File_Close();
	return TRUE;
}
/************************************************************************/
/*                        文件管理                                     */
/************************************************************************/
extern "C" BOOL SQLPacket_File_Insert(XENGINE_PROTOCOLFILE * pSt_ProtocolFile)
{
	return m_SQLFile.SQLPacket_File_Insert(pSt_ProtocolFile);
}
extern "C" BOOL SQLPacket_File_Delete(XENGINE_PROTOCOLFILE * pSt_ProtocolFile)
{
	return m_SQLFile.SQLPacket_File_Delete(pSt_ProtocolFile);
}
extern "C" BOOL SQLPacket_File_QueryAll(XENGINE_PROTOCOLFILE * **pppSt_ProtocolFile, int* pInt_ListCount)
{
	return m_SQLFile.SQLPacket_File_QueryAll(pppSt_ProtocolFile, pInt_ListCount);
}