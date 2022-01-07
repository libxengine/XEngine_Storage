#include "pch.h"
#include "XStorage_MySql/XStorage_MySql.h"
#include "XStorage_SQLite/XStorage_SQLite.h"
#include "XStorage_SQLHelp/XStorage_SQLHelp.h"
/********************************************************************
//    Created:     2021/06/10  11:10:16
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\pch.cpp
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
BOOL XStorage_IsErrorOccur = FALSE;
DWORD XStorage_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CXStorage_MySql m_MySql;
CXStorage_SQLite m_SQLite;
CXStorage_SQLHelp m_SQLHelp;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD XStorageDB_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return XStorage_dwErrorCode;
}
/************************************************************************/
/*                         导出的数据库操作函数                         */
/************************************************************************/
extern "C" BOOL XStorage_MySql_Init(DATABASE_MYSQL_CONNECTINFO * pSt_DBConnector, int nTimeMonth)
{
	return m_MySql.XStorage_MySql_Init(pSt_DBConnector, nTimeMonth);
}
extern "C" BOOL XStorage_MySql_Destory()
{
	return m_MySql.XStorage_MySql_Destory();
}
extern "C" BOOL XStorage_MySql_FileInsert(XSTORAGECORE_DBFILE * pSt_DBManage)
{
	return m_MySql.XStorage_MySql_FileInsert(pSt_DBManage);
}
extern "C" BOOL XStorage_MySql_FileDelete(LPCTSTR lpszBuckKey, LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_MySql.XStorage_MySql_FileDelete(lpszBuckKey, lpszFile, lpszHash);
}
extern "C" BOOL XStorage_MySql_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszBuckKey, LPCTSTR lpszFile, LPCTSTR lpszHash, LPCTSTR lpszTableName)
{
	return m_MySql.XStorage_MySql_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszBuckKey, lpszFile, lpszHash, lpszTableName);
}
/************************************************************************/
/*                         SQLITE数据库函数                             */
/************************************************************************/
extern "C" BOOL XStorage_SQLite_Init(LPCTSTR lpszSQLFile, int nTimeMonth)
{
	return m_SQLite.XStorage_SQLite_Init(lpszSQLFile, nTimeMonth);
}
extern "C" BOOL XStorage_SQLite_Destory()
{
	return m_SQLite.XStorage_SQLite_Destory();
}
extern "C" BOOL XStorage_SQLite_FileInsert(XSTORAGECORE_DBFILE * pSt_DBManage)
{
	return m_SQLite.XStorage_SQLite_FileInsert(pSt_DBManage);
}
extern "C" BOOL XStorage_SQLite_FileDelete(LPCTSTR lpszBuckKey, LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_SQLite.XStorage_SQLite_FileDelete(lpszBuckKey, lpszFile, lpszHash);
}
extern "C" BOOL XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszBuckKey, LPCTSTR lpszFile, LPCTSTR lpszHash, LPCTSTR lpszTableName)
{
	return m_SQLite.XStorage_SQLite_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszBuckKey, lpszFile, lpszHash, lpszTableName);
}
/************************************************************************/
/*                         SQL帮助函数集                                */
/************************************************************************/
extern "C" BOOL XStorage_SQLHelp_Insert(TCHAR * ptszSQLBuffer, XSTORAGECORE_DBFILE * pSt_DBFile)
{
	return m_SQLHelp.XStorage_SQLHelp_Insert(ptszSQLBuffer, pSt_DBFile);
}
extern "C" BOOL XStorage_SQLHelp_Delete(TCHAR * ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey, LPCTSTR lpszFileName, LPCTSTR lpszFileHash)
{
	return m_SQLHelp.XStorage_SQLHelp_Delete(ptszSQLBuffer, lpszTableName, lpszBuckKey, lpszFileName, lpszFileHash);
}
extern "C" BOOL XStorage_SQLHelp_Query(TCHAR * ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey, LPCTSTR lpszFilePath, LPCTSTR lpszFileName, LPCTSTR lpszFileHash, LPCTSTR lpszFileUser, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
	return m_SQLHelp.XStorage_SQLHelp_Query(ptszSQLBuffer, lpszTableName, lpszBuckKey, lpszFilePath, lpszFileName, lpszFileHash, lpszFileUser, lpszTimeStart, lpszTimeEnd);
}