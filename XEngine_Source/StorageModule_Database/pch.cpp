#include "pch.h"
#include "Database_Help/Database_Help.h"
#include "Database_File/Database_File.h"
/********************************************************************
//    Created:     2022/03/29  14:22:29
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
bool Database_IsErrorOccur = false;
XLONG Database_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CDatabase_Help m_SQLHelp;
CDatabase_File m_SQLFile;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG Database_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Database_dwErrorCode;
}
/************************************************************************/
/*                         导出的数据库帮助函数                         */
/************************************************************************/
extern "C" bool Database_Help_Insert(XCHAR * ptszSQLBuffer, XSTORAGECORE_DBFILE * pSt_DBFile)
{
	return m_SQLHelp.Database_Help_Insert(ptszSQLBuffer, pSt_DBFile);
}
extern "C" bool Database_Help_Delete(XCHAR * ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey, LPCXSTR lpszFilePath, LPCXSTR lpszFileName, LPCXSTR lpszFileHash)
{
	return m_SQLHelp.Database_Help_Delete(ptszSQLBuffer, lpszTableName, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash);
}
extern "C" bool Database_Help_Query(XCHAR * ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey, LPCXSTR lpszFilePath, LPCXSTR lpszFileName, LPCXSTR lpszFileHash, LPCXSTR lpszFileUser, LPCXSTR lpszTimeStart, LPCXSTR lpszTimeEnd)
{
	return m_SQLHelp.Database_Help_Query(ptszSQLBuffer, lpszTableName, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash, lpszFileUser, lpszTimeStart, lpszTimeEnd);
}
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
extern "C" bool Database_File_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth)
{
	return m_SQLFile.Database_File_Init(pSt_DBConnector, nTimeMonth);
}
extern "C" bool Database_File_Destory()
{
	return m_SQLFile.Database_File_Destory();
}
extern "C" bool Database_File_FileInsert(XSTORAGECORE_DBFILE * pSt_DBManage, bool bRewrite)
{
	return m_SQLFile.Database_File_FileInsert(pSt_DBManage, bRewrite);
}
extern "C" bool Database_File_FileDelete(LPCXSTR lpszBuckKey, LPCXSTR lpszFilePath, LPCXSTR lpszFileName, LPCXSTR lpszHash)
{
	return m_SQLFile.Database_File_FileDelete(lpszBuckKey, lpszFilePath, lpszFileName, lpszHash);
}
extern "C" bool Database_File_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszTimeStart, LPCXSTR lpszTimeEnd, LPCXSTR lpszBuckKey, LPCXSTR lpszFilePath, LPCXSTR lpszFileName, LPCXSTR lpszHash, LPCXSTR lpszTableName)
{
	return m_SQLFile.Database_File_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszBuckKey, lpszFilePath, lpszFileName, lpszHash, lpszTableName);
}