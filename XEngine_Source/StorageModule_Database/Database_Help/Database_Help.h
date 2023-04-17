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
	bool Database_Help_Insert(XCHAR* ptszSQLBuffer, XSTORAGECORE_DBFILE* pSt_DBFile);
	bool Database_Help_Delete(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL);
	bool Database_Help_Query(XCHAR* ptszSQLBuffer, LPCXSTR lpszTableName, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL, LPCXSTR lpszFileUser = NULL, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL);
protected:
	bool Database_Help_Packet(XCHAR* ptszSQLBuffer, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszFileHash = NULL, LPCXSTR lpszFileUser = NULL, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL);
};
