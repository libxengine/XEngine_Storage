#pragma once
/********************************************************************
//    Created:     2023/10/20  09:58:29
//    File Name:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Memory\Database_Memory.h
//    File Path:   D:\XEngine_Storage\XEngine_Source\StorageModule_Database\Database_Memory
//    File Base:   Database_Memory
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     内存数据库管理器
//    History:
*********************************************************************/

class CDatabase_Memory
{
public:
    CDatabase_Memory();
    ~CDatabase_Memory();
public:
    bool Database_Memory_Init(list<XENGINE_STORAGEBUCKET>* pStl_ListBucket, int nHashMode);
    bool Database_Memory_Destory();
    bool Database_Memory_FileInsert(XSTORAGECORE_DBFILE *pSt_DBManage);
    bool Database_Memory_FileDelete(LPCXSTR lpszFilePath = NULL, LPCXSTR lpszFileName = NULL, LPCXSTR lpszHash = NULL);
    bool Database_Memory_FileQuery(XSTORAGECORE_DBFILE*** pppSt_ListFile, int* pInt_ListCount, LPCXSTR lpszBucketName, LPCXSTR lpszFileName, LPCXSTR lpszHash);
protected:
    bool Database_Memory_Flush();
protected:
    static XHTHREAD Database_Memory_Thread(XPVOID lParam);
private:
    bool m_bIsRun = false;
    int m_nHashMode = 0;
private:
    unique_ptr<shared_mutex> st_Locker;
    unique_ptr<std::thread> pSTDThread;
private:
    list<XSTORAGECORE_DBFILE> stl_ListFile;
    list<XENGINE_STORAGEBUCKET> stl_ListBucket;
};
