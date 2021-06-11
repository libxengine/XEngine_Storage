#pragma once
/********************************************************************
//    Created:     2021/06/10  11:09:09
//    File Name:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_File\XStorageSQL_File.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\XEngine_StorageComponents\XStorage_SQLPacket\XStorageSQL_File
//    File Base:   XStorageSQL_File
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     文件管理器
//    History:
*********************************************************************/

class CXStorageSQL_File
{
public:
    CXStorageSQL_File();
    ~CXStorageSQL_File();
public:
    BOOL XStorageSQL_File_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector, int nTimeMonth = 1);
    BOOL XStorageSQL_File_Destory();
    BOOL XStorageSQL_File_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
    BOOL XStorageSQL_File_FileDelete(LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
    BOOL XStorageSQL_File_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL, LPCTSTR lpszFile = NULL, LPCTSTR lpszHash = NULL);
    BOOL XStorageSQL_File_FileQueryForTable(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCTSTR lpszTableName);
    BOOL XStorageSQL_File_FileQueryForHash(XSTORAGECORE_DBFILE* pSt_FileInfo, LPCTSTR lpszFileHash, LPCTSTR lpszUser = NULL, LPCTSTR lpszTimeStart = NULL, LPCTSTR lpszTimeEnd = NULL);
    BOOL XStorageSQL_File_FileGetCount(__int64x *pInt_Count, __int64x *pInt_Size);
protected:
    BOOL XStorageSQL_File_CreateTable();
    BOOL XStorageSQL_File_TimeMonth(LPCTSTR lpszStartTime, int* pInt_Month);
    BOOL XStorageSQL_File_TimeDel();
protected:
    static XHTHREAD XStorageSQL_File_Thread(LPVOID lParam);
private:
    BOOL bIsRun;
    int m_nTimeMonth;
    XHDATA xhDBSQL;
private:
    shared_ptr<std::thread> pSTDThread;
};
