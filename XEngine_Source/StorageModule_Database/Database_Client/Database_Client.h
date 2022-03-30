#pragma once
/********************************************************************
//    Created:     2022/03/29  15:35:41
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Client\Database_Client.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Client
//    File Base:   Database_Client
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端数据库操作类
//    History:
*********************************************************************/

class CDatabase_Client
{
public:
	CDatabase_Client();
	~CDatabase_Client();
public:
	BOOL Database_Client_Init(LPCTSTR lpszSQLFile);
	BOOL Database_Client_Destory();
	BOOL Database_Client_FileInsert(XSTORAGECORE_DBFILE* pSt_DBManage);
	BOOL Database_Client_FileDelete(LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
	BOOL Database_Client_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
protected:
	BOOL Database_Client_CreateTable(LPCTSTR lpszTableName);
private:
	TCHAR tszTableName[MAX_PATH];
	XHDATA xhSQL;
};