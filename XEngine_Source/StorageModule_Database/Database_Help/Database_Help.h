#pragma once
/********************************************************************
//    Created:     2022/03/30  11:20:08
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Help\Database_Help.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Help
//    File Base:   Database_Help
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库语法帮助类
//    History:
*********************************************************************/

class CDatabase_Help
{
public:
	CDatabase_Help();
	~CDatabase_Help();
public:
	BOOL Database_Help_Insert(TCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile);
	BOOL Database_Help_Delete(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL);
	BOOL Database_Help_Query(TCHAR* ptszSQLBuffer, LPCTSTR lpszTableName, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFilePath = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL, LPCTSTR lpszFileUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
protected:
	BOOL Database_Help_Packet(TCHAR* ptszSQLBuffer, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFilePath = NULL, LPCTSTR lpszFileName = NULL, LPCTSTR lpszFileHash = NULL, LPCTSTR lpszFileUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
};
