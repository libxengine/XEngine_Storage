#include "pch.h"
#include "SQLPacket_File.h"
/********************************************************************
//    Created:     2021/06/09  11:18:37
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\SQLPacket_File\SQLPacket_File.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_SQLPacket\SQLPacket_File
//    File Base:   SQLPacket_File
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件管理
//    History:
*********************************************************************/
CSQLPacket_File::CSQLPacket_File()
{

}
CSQLPacket_File::~CSQLPacket_File()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：SQLPacket_File_Connect
函数功能：连接MYSQL数据库
 参数.一：lpszSQLAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：数据库地址
 参数.二：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：端口
 参数.三：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：用户名
 参数.四：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：密码
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSQLPacket_File::SQLPacket_File_Connect(LPCTSTR lpszSQLAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	SQLPacket_IsErrorOccur = FALSE;

	if ((NULL == lpszSQLAddr) || (NULL == lpszUser) || (NULL == lpszPass))
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_PARAMENT;
		return FALSE;
	}
	DATABASE_MYSQL_CONNECTINFO st_MySQLConnect;
	memset(&st_MySQLConnect, '\0', sizeof(DATABASE_MYSQL_CONNECTINFO));

	st_MySQLConnect.nPort = nPort;
	_tcscpy(st_MySQLConnect.tszHostAddr, lpszSQLAddr);
	_tcscpy(st_MySQLConnect.tszName, lpszUser);
	_tcscpy(st_MySQLConnect.tszPassWord, lpszPass);
	_tcscpy(st_MySQLConnect.tszDBName, _T("XEngine_Storage"));

	if (!DataBase_MySQL_Connect(&xhSQL, &st_MySQLConnect))
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：SQLPacket_File_Close
函数功能：关闭连接,清理资源
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSQLPacket_File::SQLPacket_File_Close()
{
	SQLPacket_IsErrorOccur = FALSE;

	DataBase_MySQL_Close(xhSQL);
	return TRUE;
}
/********************************************************************
函数名称：SQLPacket_File_Insert
函数功能：插入一条记录
 参数.一：pSt_ProtocolFile
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入文件信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSQLPacket_File::SQLPacket_File_Insert(XENGINE_PROTOCOLFILE* pSt_ProtocolFile)
{
	SQLPacket_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolFile)
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[8196];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T("INSERT INTO `XStorage_File`(FilePath,FileName,FileHash,FileUser,FileSize,FileTime) VALUES('%s','%s','%s','%s',%lld,now())"), pSt_ProtocolFile->tszFilePath, pSt_ProtocolFile->tszFileName, pSt_ProtocolFile->tszFileHash, pSt_ProtocolFile->tszFileUser, pSt_ProtocolFile->nFileSize);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：SQLPacket_File_Delete
函数功能：删除一条记录
 参数.一：pSt_ProtocolFile
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要删除的记录
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSQLPacket_File::SQLPacket_File_Delete(XENGINE_PROTOCOLFILE* pSt_ProtocolFile)
{
	SQLPacket_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolFile)
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[8196];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	if (_tcslen(pSt_ProtocolFile->tszFileHash) > 0)
	{
		_stprintf_s(tszSQLQuery, _T("DELETE FROM `XStorage_File` WHERE FileHash = '%s'"), pSt_ProtocolFile->tszFileHash);
	}
	else if (_tcslen(pSt_ProtocolFile->tszFileName) > 0)
	{
		_stprintf_s(tszSQLQuery, _T("DELETE FROM `XStorage_File` WHERE FilePath = '%s' AND FileName = '%s'"), pSt_ProtocolFile->tszFilePath, pSt_ProtocolFile->tszFileName);
	}
	
	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：SQLPacket_File_QueryAll
函数功能：查询所有文件
 参数.一：pppSt_ProtocolFile
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出文件列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出文件个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CSQLPacket_File::SQLPacket_File_QueryAll(XENGINE_PROTOCOLFILE*** pppSt_ProtocolFile, int* pInt_ListCount)
{
	SQLPacket_IsErrorOccur = FALSE;

	if (NULL == pppSt_ProtocolFile)
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_PARAMENT;
		return FALSE;
	}
	DWORD64 dwLine = 0;
	DWORD64 dwField = 0;
	XHDATA xhResult = 0;

	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf(tszSQLQuery, _T("SELECT * FROM XStorage_File"));

	if (!DataBase_MySQL_ExecuteQuery(xhSQL, &xhResult, tszSQLQuery, &dwLine, &dwField))
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	if (dwLine <= 0)
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_NODATA;
		return FALSE;
	}
	TCHAR** pptszResult = DataBase_MySQL_GetResult(xhSQL, xhResult);
	if (NULL == pptszResult[0])
	{
		SQLPacket_IsErrorOccur = TRUE;
		SQLPacket_dwErrorCode = ERROR_STORAGE_MODULE_SQLPACKET_NOTVALUE;
		return FALSE;
	}
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ProtocolFile, (int)dwLine, sizeof(XENGINE_PROTOCOLFILE));

	for (DWORD64 i = 0; i < dwLine; i++)
	{
		if (NULL != pptszResult[i + 1])
		{
			_tcscpy((*pppSt_ProtocolFile)[i]->tszFilePath, pptszResult[i + 1]);
		}
		if (NULL != pptszResult[i + 2])
		{
			_tcscpy((*pppSt_ProtocolFile)[i]->tszFileName, pptszResult[i + 2]);
		}
		if (NULL != pptszResult[i + 3])
		{
			_tcscpy((*pppSt_ProtocolFile)[i]->tszFileHash, pptszResult[i + 3]);
		}
		if (NULL != pptszResult[i + 4])
		{
			_tcscpy((*pppSt_ProtocolFile)[i]->tszFileUser, pptszResult[i + 4]);
		}
		if (NULL != pptszResult[i + 5])
		{
			(*pppSt_ProtocolFile)[i]->nFileSize = _ttoi64(pptszResult[i + 5]);
		}
		if (NULL != pptszResult[i + 6])
		{
			_tcscpy((*pppSt_ProtocolFile)[i]->tszFileTime, pptszResult[i + 6]);
		}
	}
	DataBase_MySQL_FreeResult(xhSQL, xhResult);
	return TRUE;
}