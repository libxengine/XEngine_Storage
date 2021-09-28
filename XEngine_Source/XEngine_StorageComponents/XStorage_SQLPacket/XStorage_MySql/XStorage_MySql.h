#pragma once
/********************************************************************
//    Created:     2021/09/28  15:49:29
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql\XStorage_MySql.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorage_MySql
//    File Base:   XStorage_MySql
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     MYSQL数据库管理器
//    History:
*********************************************************************/

class CXStorage_MySql
{
public:
    CXStorage_MySql();
    ~CXStorage_MySql();
public:
    BOOL XStorage_MySql_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, int nTimeMonth = 1);
    BOOL XStorage_MySql_Destory();
    BOOL XStorage_MySql_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
    BOOL XStorage_MySql_FileDelete(LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
    BOOL XStorage_MySql_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
    BOOL XStorage_MySql_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName);
    BOOL XStorage_MySql_FileQueryForHash(XSTORAGECORE_DBFILE* pSt_FileInfo, LPCTSTR lpszFileHash, LPCTSTR lpszUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
    BOOL XStorage_MySql_FileGetCount(__int64x *pInt_Count, __int64x *pInt_Size);
protected:
    BOOL XStorage_MySql_CreateTable();
    BOOL XStorage_MySql_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month);
    BOOL XStorage_MySql_TimeDel();
protected:
    static XHTHREAD XStorage_MySql_Thread(LPVOID lParam);
private:
    BOOL bIsRun;
    int m_nTimeMonth;
    XHDATA xhDBSQL;
private:
    shared_ptr<std::thread> pSTDThread;
};
