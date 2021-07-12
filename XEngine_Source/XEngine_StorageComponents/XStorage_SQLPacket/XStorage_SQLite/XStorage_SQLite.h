#pragma once
/********************************************************************
//    Created:     2021/07/12  09:31:03
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite\XStorage_SQLite.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_SQLite
//    File Base:   XStorage_SQLite
//    File Ext:    h
//    Project:     XEngine(����ͨ������)
//    Author:      qyt
//    Purpose:     SQLITE������
//    History:
*********************************************************************/


class CXStorage_SQLite
{
public:
	CXStorage_SQLite();
	~CXStorage_SQLite();
public:
	BOOL XStorage_SQLite_Init(LPCTSTR lpszSQLFile, int nTimeMonth = 1);
	BOOL XStorage_SQLite_Destory();
	BOOL XStorage_SQLite_FileInsert(XSTORAGECORE_DBFILE* pSt_DBManage);
	BOOL XStorage_SQLite_FileDelete(LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
	BOOL XStorage_SQLite_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
	BOOL XStorage_SQLite_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName);
protected:
	BOOL XStorage_SQLite_CreateTable();
	BOOL XStorage_SQLite_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month);
	BOOL XStorage_SQLite_TimeDel();
	BOOL XStorage_SQLite_IsNumber(LPCTSTR lpszStr);
protected:
	static XHTHREAD XStorage_SQLite_Thread(LPVOID lParam);
private:
	BOOL bIsRun;
	int m_nTimeMonth;
	XHDATA xhSQL;
private:
	shared_ptr<std::thread> pSTDThread;
};