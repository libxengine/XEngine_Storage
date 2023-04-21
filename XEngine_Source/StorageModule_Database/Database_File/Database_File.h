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
    bool Database_File_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, int nTimeMonth = 1);
    bool Database_File_Destory();
    bool Database_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage, bool bRewrite = false);
    bool Database_File_FileDelete(LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL);
    bool Database_File_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszTimeStart = NULL, LPCXSTR lpszTimeEnd = NULL, LPCXSTR lpszBuckKey = NULL, LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL, LPCXSTR lpszTableName = NULL);
protected:
    bool Database_File_CreateTable();
    bool Database_File_TimeMonth(LPCXSTR lpszStartTime, int* pInt_Month);
    bool Database_File_TimeDel();
protected:
    static XHTHREAD Database_File_Thread(XPVOID lParam);
private:
    bool bIsRun;
    int m_nTimeMonth;
    XNETHANDLE xhDBSQL;
private:
    shared_ptr<std::thread> pSTDThread;
};
