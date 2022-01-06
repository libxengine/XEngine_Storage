#include "pch.h"
#include "XStorage_MySql/XStorage_MySql.h"
#include "XStorage_SQLite/XStorage_SQLite.h"
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
extern "C" BOOL XStorage_MySql_FileDelete(LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_MySql.XStorage_MySql_FileDelete(lpszFile, lpszHash);
}
extern "C" BOOL XStorage_MySql_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_MySql.XStorage_MySql_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszFile, lpszHash);
}
extern "C" BOOL XStorage_MySql_FileQueryForTable(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
{
	return m_MySql.XStorage_MySql_FileQueryForTable(pppSt_ListFile, pInt_ListCount, lpszTableName);
}
extern "C" BOOL XStorage_MySql_FileQueryForHash(XSTORAGECORE_DBFILE * pSt_FileInfo, LPCTSTR lpszFileMD5, LPCTSTR lpszUser, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd)
{
	return m_MySql.XStorage_MySql_FileQueryForHash(pSt_FileInfo, lpszFileMD5, lpszUser, lpszTimeStart, lpszTimeEnd);
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
extern "C" BOOL XStorage_SQLite_FileDelete(LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_SQLite.XStorage_SQLite_FileDelete(lpszFile, lpszHash);
}
extern "C" BOOL XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart, LPCTSTR lpszTimeEnd, LPCTSTR lpszFile, LPCTSTR lpszHash)
{
	return m_SQLite.XStorage_SQLite_FileQuery(pppSt_ListFile, pInt_ListCount, lpszTimeStart, lpszTimeEnd, lpszFile, lpszHash);
}
extern "C" BOOL XStorage_SQLite_FileQueryForTable(XSTORAGECORE_DBFILE * **pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName)
{
	return m_SQLite.XStorage_SQLite_FileQueryForTable(pppSt_ListFile, pInt_ListCount, lpszTableName);
}