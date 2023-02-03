#pragma once
/********************************************************************
//    Created:     2022/03/29  14:07:06
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_File\Database_File.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_File
//    File Base:   Database_File
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件数据库管理器
//    History:
*********************************************************************/

class CDatabase_File
{
public:
    CDatabase_File();
    ~CDatabase_File();
public:
    BOOL Database_File_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, int nTimeMonth = 1);
    BOOL Database_File_Destory();
    BOOL Database_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
    BOOL Database_File_FileDelete(LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
    BOOL Database_File_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszBuckKey = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL, LPCTSTR lpszTableName = NULL);
protected:
    BOOL Database_File_CreateTable();
    BOOL Database_File_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month);
    BOOL Database_File_TimeDel();
protected:
    static XHTHREAD Database_File_Thread(LPVOID lParam);
private:
    BOOL bIsRun;
    int m_nTimeMonth;
    XNETHANDLE xhDBSQL;
private:
    shared_ptr<std::thread> pSTDThread;
};
